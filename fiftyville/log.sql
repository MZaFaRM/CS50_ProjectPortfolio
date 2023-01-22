-- Keep a log of any SQL queries you execute as you solve the mystery.

-- What I currently know theft took place on July 28, 2021 and it took place on Humphrey Street.

-- Checked for a crime with similar description from crime_scene_reports
SELECT description FROM crime_scene_reports WHERE street = "Humphrey Street" AND day = 28;

/*
Result:
        Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were
        present at the time – each of their interview transcripts mentions the bakery.
*/

-- Interviews from Day 28 where "Bakery" was mentioned
SELECT transcript FROM interviews WHERE day = 28 AND transcript LIKE "%Bakery%";

/*
Result:
        +------------------------------------------------------------------------------------------------------------------------------------------+
        |                                                           transcript                                                                     |
        +------------------------------------------------------------------------------------------------------------------------------------------+
        | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away.                       |
        | If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.  |
        |                                                                                                                                          |
        | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery,                 |
        | I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.                                               |
        |                                                                                                                                          |
        | As the thief was leaving the bakery, they called someone who talked to them for less than a minute.                                      |
        | In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.                       |
        | The thief then asked the person on the other end of the phone to purchase the flight ticket.                                             |
        +------------------------------------------------------------------------------------------------------------------------------------------+


Statement 1:
        Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away.
        If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.
*/

-- Cars that left the parking lot in between 10:15 and 10:25
SELECT license_plate FROM bakery_security_logs WHERE hour = 10 AND activity = "exit" BETWEEN 15 AND 25;

/*  Result:
+---------------+
| license_plate |
+---------------+
| 5P2BI95       |
| 94KL13X       |
| 6P58WS2       |
| 4328GD8       |
| G412CB7       |
| L93JTIZ       |
| 322W7JE       |
| 0NTHK55       |
| PF37ZVK       |
| 1M92998       |
| XE95071       |
| IH61GO8       |
| 8P9NEU9       |
+---------------+


Statement 2:
        I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery,
        I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.
*/
SELECT id, account_number, amount FROM atm_transactions WHERE day = 28 AND month = 7 AND year = 2021 AND atm_location = "Leggett Street" AND transaction_type = "withdraw";

/*
Result:
        +----------------+--------+
        | account_number | amount |
        +----------------+--------+
        | 28500762       | 48     |
        | 28296815       | 20     |
        | 76054385       | 60     |
        | 49610011       | 50     |
        | 16153065       | 80     |
        | 25506511       | 20     |
        | 81061156       | 30     |
        | 26013199       | 35     |
        +----------------+--------+

Statement 3:
As the thief was leaving the bakery, they called someone who talked to them for less than a minute.
In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow (29).
The thief then asked the person on the other end of the phone to purchase the flight ticket.
*/

SELECT caller, receiver FROM phone_calls WHERE day = 28 AND month = 7 AND year = 2021 AND duration <= 60;

/*
Result:
        +----------------+----------------+
        |     caller     |    receiver    |
        +----------------+----------------+
        | (130) 555-0289 | (996) 555-8899 |
        | (499) 555-9472 | (892) 555-8872 |
        | (367) 555-5533 | (375) 555-8161 |
        | (609) 555-5876 | (389) 555-5198 |
        | (499) 555-9472 | (717) 555-1342 |
        | (286) 555-6063 | (676) 555-6554 |
        | (770) 555-1861 | (725) 555-3243 |
        | (031) 555-6622 | (910) 555-3251 |
        | (826) 555-1652 | (066) 555-9701 |
        | (338) 555-6650 | (704) 555-2131 |
        +----------------+----------------+
*/
-- From all the information given from the witness we get
SELECT name FROM people WHERE phone_number IN (SELECT caller FROM phone_calls WHERE day = 28 AND month = 7 AND year = 2021 AND duration <= 60);
SELECT name FROM people WHERE license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE hour = 10 AND activity = "exit" AND minute BETWEEN 15 AND 25);
SELECT name FROM people WHERE id IN (SELECT person_id FROM bank_accounts WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE day = 28 AND month = 7 AND year = 2021 AND atm_location = "Leggett Street" AND transaction_type = "withdraw"));

-- Combining we get
SELECT name, passport_number FROM people WHERE phone_number IN (SELECT caller FROM phone_calls WHERE day = 28 AND month = 7 AND year = 2021 AND duration <= 60) INTERSECT
SELECT name, passport_number FROM people WHERE license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE hour = 10 AND activity = "exit" AND minute BETWEEN 15 AND 25) INTERSECT
SELECT name, passport_number FROM people WHERE id IN (SELECT person_id FROM bank_accounts WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE day = 28 AND month = 7 AND year = 2021 AND atm_location = "Leggett Street" AND transaction_type = "withdraw"));

/*
Narrowing the suspects list between
+-------+
| name  |
+-------+
| Bruce |
| Diana |
+-------+
*/

SELECT passport_number FROM passengers WHERE flight_id IN (SELECT id FROM flights WHERE day = 29) INTERSECT
SELECT passport_number FROM people WHERE phone_number IN (SELECT caller FROM phone_calls WHERE day = 28 AND month = 7 AND year = 2021 AND duration <= 60) INTERSECT
SELECT passport_number FROM people WHERE license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE hour = 10 AND activity = "exit" AND minute BETWEEN 15 AND 25) INTERSECT
SELECT passport_number FROM people WHERE id IN (SELECT person_id FROM bank_accounts WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE day = 28 AND month = 7 AND year = 2021 AND atm_location = "Leggett Street" AND transaction_type = "withdraw"));

-- Both Bruce and Diana did almost the exact same things that day, hence no change in suspect list.

-- Who did Bruce call?

SELECT DISTINCT people.name, phone_calls.receiver FROM people JOIN phone_calls
WHERE people.phone_number
IN (SELECT receiver FROM phone_calls WHERE caller IN (SELECT phone_number FROM people WHERE name LIKE "Bruce") INTERSECT
SELECT receiver FROM phone_calls WHERE day = 28 AND month = 7 AND year = 2021 AND duration <= 60)
AND phone_calls.receiver = people.phone_number;

 /*
+--------+---------------+
| name  |    receiver    |
+-------+----------------+
| Robin | (375) 555-8161 |
+-------+----------------+
*/

-- Who did Diana call?

SELECT DISTINCT people.name, phone_calls.receiver FROM people JOIN phone_calls
WHERE people.phone_number
IN (SELECT receiver FROM phone_calls WHERE caller IN (SELECT phone_number FROM people WHERE name LIKE "DIANA") INTERSECT
SELECT receiver FROM phone_calls WHERE day = 28 AND month = 7 AND year = 2021 AND duration <= 60)
AND phone_calls.receiver = people.phone_number;

 /*
+--------+----------------+
|  name  |    receiver    |
+--------+----------------+
| Philip | (725) 555-3243 |
+--------+----------------+
*/

-- Earliest flight out of fiftyville
SELECT destination_airport_id, hour, minute FROM flights WHERE origin_airport_id IN
(SELECT id FROM airports WHERE city LIKE "Fiftyville") AND day = 29 AND month = 7 AND year = 2021 ORDER BY hour LIMIT 1;

-- Are any of the suspects in this flight?

SELECT passport_number FROM people WHERE name LIKE "Diana" INTERSECT
SELECT passport_number FROM passengers WHERE flight_id IN (SELECT id FROM flights WHERE origin_airport_id IN
(SELECT id FROM airports WHERE city LIKE "Fiftyville") AND hour = 8);
-- Returns Nothing

SELECT passport_number FROM people WHERE name LIKE "flight" INTERSECT
SELECT passport_number FROM passengers WHERE flight_id IN (SELECT id FROM flights WHERE origin_airport_id IN
(SELECT id FROM airports WHERE city LIKE "Fiftyville") AND hour = 8);
-- Proves Bruce was in this flight

-- Where did Bruce fly to?

SELECT city FROM airports WHERE id IN (SELECT destination_airport_id FROM flights WHERE origin_airport_id IN
(SELECT id FROM airports WHERE city LIKE "Fiftyville") AND day = 29 AND month = 7 AND year = 2021 ORDER BY hour LIMIT 1);

-- Shows he flew to New York

-- Hence, it has been proven that Bruce did indeed take the earliest flight out of Fiftyville and flew to New York,
-- with the help of his accomplice Robin whom Bruce called to book the flight.
