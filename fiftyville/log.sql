-- Keep a log of any SQL queries you execute as you solve the mystery.
-- Check created tables
.tables
-- Check first important table
.schema crime_scene_reports
-- Information we have: date and location
-- Search for cases maching date and location, extract all to see description and id
SELECT * FROM crime_scene_reports WHERE year = 2021 AND month = 7 AND day = 28 AND street = 'Humphrey Street';
-- Relevant information: id = 295, time = 10:15am, location: bakery, interviews from 3 people which transcripts mention the bakery
-- Check bakery_security_logs table
.schema bakery_security_logs
-- Check interviews
.schema interviews
-- Check all relevant inerviews
SELECT * FROM interviews WHERE year = 2021 AND month = 7 AND day = 28 AND transcript LIKE '%bakery%';
-- Thief left within 10 minutes (i.e. up to 10:25am)
-- Thief was withdrawing money by the ATM on Leggett Street earlier this morning (i.e. before 10:15am)
-- When leaving bakery, called someone for less than a minute. Earliest flight out of Fiftyville tomorrow (i.e. 29.07.2021), the other person purchased the ticket.
-- Check who was leaving bakery 10:15am-10:25am
SELECT * FROM bakery_security_logs WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10;
-- We know 8 license plates, one of which belongs to the thief
-- Check ATM table
.schema atm_transactions
-- Check airports table
SELECT * FROM airports WHERE city = 'Fiftyville';
-- There is only one airport in the city, with id = 8
-- Check flights table
.schema flights
-- Check possible flights
SELECT * FROM flights WHERE origin_airport_id = 8 AND year = 2021 AND month = 7 AND day = 29;
-- There are 4 flights, the thief took the earliest one (i.e. at 8:20am) so the id is 36;
-- Id of destination airport is 4
-- IDENTIFY WHERE THE THE THIEF ESCAPED TO
SELECT city FROM airports WHERE id = 4;
-- LOCATION: New York City
-- Check passengers table
.schema passengers
-- To get all passport_numbers of possible thief we can execute
SELECT passport_number FROM passengers WHERE flight_id = 36;
-- Check people table
.schema people
-- We can connect thief with his passport_number, phone_number and license_plate;
-- Check phone calls table
.schema phone_calls
-- IDENTIFY THIEF
SELECT name
FROM people
WHERE passport_number IN
(
    SELECT passport_number
    FROM passengers
    WHERE flight_id = 36
)
AND license_plate IN
(
    SELECT license_plate
    FROM bakery_security_logs
    WHERE year = 2021
    AND month = 7
    AND day = 28
    AND hour = 10
    AND minute > 15
    AND minute < 25
);
-- Not enough information
-- Check ATM withdrawals
SELECT * FROM atm_transactions  WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw';
-- Check phone calls
SELECT * FROM phone_calls WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60;
-- Connect and identify the thief
SELECT name
FROM people
WHERE passport_number IN
(
    SELECT passport_number
    FROM passengers
    WHERE flight_id = 36
)
AND license_plate IN
(
    SELECT license_plate
    FROM bakery_security_logs
    WHERE year = 2021
    AND month = 7
    AND day = 28
    AND hour = 10
    AND minute > 15
    AND minute < 25
)
AND phone_number IN
(
    SELECT caller
    FROM phone_calls
    WHERE year = 2021
    AND month = 7
    AND day = 28
    AND duration < 60
)
AND id IN
(
    SELECT person_id
    FROM bank_accounts
    WHERE account_number IN
    (
        SELECT account_number
        FROM atm_transactions
        WHERE year = 2021
        AND month = 7
        AND day = 28
        AND atm_location = 'Leggett Street'
        AND transaction_type = 'withdraw'
    )
);
-- THIEF IDENTIFIED: Bruce
-- Get accomplice phone number
SELECT name
FROM people
WHERE phone_number =
(
    SELECT receiver
    FROM phone_calls
    WHERE caller =
    (
        SELECT phone_number
        FROM people
        WHERE name = 'Bruce'
    )
    AND year = 2021
    AND month = 7
    AND day = 28
    AND duration < 60
);
-- ACCOMPLICE IDENTIFIED: Robin

