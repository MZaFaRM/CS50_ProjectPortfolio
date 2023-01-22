import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    try:
        # Removes duplicate symbols
        symbols = db.execute("SELECT symbol, shares, price, total FROM purchase WHERE user_id = ?", session["user_id"])
        all_symbols = []
        for symbol in symbols:
            if symbol["symbol"] not in all_symbols:
                all_symbols.append(symbol["symbol"])

        # Removing stocks with no shares
        bought_symbols = []
        for symbol in all_symbols:
            shares = db.execute("SELECT SUM(shares) FROM purchase WHERE symbol = ? AND user_id = ?", symbol, session["user_id"])
            shares = int(shares[0]["SUM(shares)"])
            if shares != 0:
                bought_symbols.append(symbol)

        # stores the symbols merging duplicates
        sum_symbols = {}
        current_person = []
        for symbol in bought_symbols:
            sum_symbols["symbol"] = symbol
            current_person.append(sum_symbols)
            sum_symbols = {}

        # adds the sum of shares of symbols
        for data in current_person:

            user_id = int(session["user_id"])

            symbols = db.execute("SELECT SUM(shares) FROM purchase WHERE user_id = ? AND symbol LIKE ?", user_id, data["symbol"])

            data["shares"] = symbols[0]["SUM(shares)"]

        TOTAL = 0

        for data in current_person:
            symbol = lookup(data["symbol"])
            data["name"] = symbol["name"]
            data["price"] = symbol["price"]
            data["total"] = data["shares"] * data["price"]

            TOTAL = TOTAL + data["total"]

        BALANCE = db.execute("SELECT cash FROM users WHERE id = ?", user_id)

        BALANCE = BALANCE[0]["cash"]

        TOTAL = TOTAL + BALANCE

        return render_template("index.html", details=current_person, balance=BALANCE, total=TOTAL)

    except:

        user_id = int(session["user_id"])
        BALANCE = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
        BALANCE = BALANCE[0]["cash"]

        return render_template("index.html", balance=BALANCE, total=BALANCE)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # Gets symbol and checks it
        symbol = request.form.get("symbol")

        symbol = lookup(symbol)

        if not symbol:
            return apology("invalid symbol", 400)

        # Gets the shares
        shares = request.form.get("shares")

        if not shares.isdigit():
            return apology("Invalid shares", 400)
        else:
            shares = int(shares)

        # Verifies the shares
        if shares < 0:
            return apology("invalid amount", 400)

        # gets the user balance
        balance = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        balance = balance[0]

        if (shares * symbol["price"]) > balance["cash"]:
            return apology("Not enough balance", 400)

        # collects all the data required for adding to table
        user_id = session["user_id"]

        # Time of purchase
        now = datetime.now()

        # total of purchase
        total = shares * symbol["price"]

        # Inserts it into the table
        db.execute("INSERT INTO purchase (user_id, symbol, shares, price, total, time) VALUES (?, ?, ?, ?, ?, ?)",
                   user_id, symbol["symbol"], shares, symbol["price"], total, now)

        balance = db.execute("SELECT cash FROM users WHERE id = ?", user_id)

        balance = balance[0]["cash"] - shares * symbol["price"]

        db.execute("UPDATE users SET cash = ? WHERE id = ?", balance, user_id)

        # Return to homepage if all goes well
        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    try:
        data = db.execute("SELECT * FROM purchase WHERE user_id = ?", session["user_id"])
        purchase = {}
        datas = []
        for i in data:
            purchase["symbol"] = i["symbol"]
            purchase["shares"] = i["shares"]
            purchase["price"] = i["price"]
            purchase["total"] = i["total"]
            purchase["time"] = i["time"]
            datas.append(purchase)
            purchase = {}

        return render_template("history.html", details=datas)
    except:
        return render_template("history.html")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 400)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":

        # Gets symbol and checks it
        SYMBOL = lookup(request.form.get("symbol"))
        if not SYMBOL:
            return apology("invalid symbol", 400)

        return render_template("quoted.html", symbol=SYMBOL)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    # Gets password and confirmation and checks if both are same
    if request.method == "POST":

        try:

            # Collects the input data
            username = request.form.get("username")
            password = request.form.get("password")
            confirmation = request.form.get("confirmation")

            # Checks for username
            if not username:
                return apology("Where username?", 400)

            # Checks for password
            if not password:
                return apology("Where password?", 400)

            # Checks for confirmation
            if not confirmation:
                return apology("Where confirms?", 400)

            # Checks if both password and cnfirmation are the same
            if password != confirmation:
                return apology("password != confirms", 400)

            # Generate hash
            hash = generate_password_hash(password, method='pbkdf2:sha256', salt_length=8)

            # Add data to table users
            db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, hash)

            # Return to homepage if all goes well
            return redirect("/")

        except ValueError:
            return apology("Username already in use")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")

        share = int(request.form.get("shares"))

        symbol = lookup(symbol)

        if not symbol:
            return apology("Invalid Symbol")
        if not share:
            return apology("Where Shares?")

        shares = db.execute("SELECT SUM(shares) FROM purchase WHERE user_id = ? AND symbol = ?",
                            session["user_id"], symbol["symbol"])

        shares = shares[0]["SUM(shares)"]

        if share > shares:
            return apology("Not enough shares to sell")

        # collects all the data required for adding to table
        user_id = session["user_id"]

        # Time of purchase
        now = datetime.now()

        # total of purchase
        total = share * symbol["price"]

        # Inserts it into the table
        db.execute("INSERT INTO purchase (user_id, symbol, shares, price, total, time) VALUES (?, ?, ?, ?, ?, ?)",
                   user_id, symbol["symbol"], (0 - share), symbol["price"], total, now)

        balance = db.execute("SELECT cash FROM users WHERE id = ?", user_id)

        balance = (balance[0]["cash"]) + (share * symbol["price"])

        db.execute("UPDATE users SET cash = ? WHERE id = ?", balance, user_id)

        # Return to homepage if all goes well
        return redirect("/")

    else:
        try:
            # Removes duplicate symbols
            symbols = db.execute("SELECT symbol FROM purchase WHERE user_id = ?", session["user_id"])
            all_symbols = []
            for symbol in symbols:
                if symbol["symbol"] not in all_symbols:
                    all_symbols.append(symbol["symbol"])

            # Removing stocks with no shares
            bought_symbols = []
            for symbol in all_symbols:
                shares = db.execute("SELECT SUM(shares) FROM purchase WHERE symbol = ? AND user_id = ?", symbol, session["user_id"])
                shares = int(shares[0]["SUM(shares)"])
                if shares != 0:
                    bought_symbols.append(symbol)
        except:
            pass

        return render_template("sell.html", symbols=bought_symbols)


@app.route("/account", methods=["GET", "POST"])
@login_required
def account():
    """Show account details"""
    if request.method == "POST":
        amount = int(request.form.get("balance"))
        if amount < 0:
            return apology("Invalid amount")

        BALANCE = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        BALANCE = BALANCE[0]["cash"]
        BALANCE = BALANCE + amount

        db.execute("UPDATE users SET cash = ? WHERE id = ?", BALANCE, session["user_id"])
        return redirect("/")
    else:
        try:
            balance = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
            balance = balance[0]["cash"]
            return render_template("account.html", balance=balance)
        except:
            return render_template("account.html")
