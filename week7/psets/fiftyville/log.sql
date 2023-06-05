-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Gets the crime scene row from the CS50 duck theft.
SELECT *
  FROM crime_scene_reports
 WHERE day = 28 AND month = 7 AND year = 2021
   AND street LIKE "Humphrey Street"
   AND description LIKE "%duck%";

-- Gets the name and transcript from the three witnesses.
SELECT name, transcript
  FROM interviews
 WHERE day = 28 AND month = 7 AND year = 2021
   AND transcript LIKE "%bakery%";

-- Gets the people that left the bakery between 10:15 and 10:25 min that day.
SELECT *
  FROM people
 WHERE people.license_plate IN
     (SELECT license_plate
        FROM bakery_security_logs
       WHERE day = 28 AND month = 7 AND year = 2021
         AND hour = 10 AND minute BETWEEN 15 AND 25
         AND activity LIKE "exit");

-- Gets the people who withdrawed money in the Legget Street that day.
SELECT *
  FROM people
 WHERE people.id IN
    (SELECT person_id
       FROM bank_accounts
      WHERE bank_accounts.account_number IN
          (SELECT account_number
             FROM atm_transactions
            WHERE day = 28 AND month = 7 AND year = 2021
              AND atm_location LIKE "Leggett Street"
              AND transaction_type LIKE "withdraw"));

-- Gets the people who did phone calls that lasted less than 60 seconds in that day.
SELECT *
  FROM people
 WHERE EXISTS
    (SELECT *
       FROM phone_calls
      WHERE day = 28 AND month = 7 AND year = 2021
        AND duration <= 60
        AND (caller = people.phone_number));

-- Gets the people who are passengers of the earliest flight out of Fiftyville from the next day.
SELECT people.id
  FROM people
 WHERE people.passport_number IN
    (SELECT passport_number
       FROM passengers
       JOIN flights ON passengers.flight_id = flights.id
       WHERE flights.id IN
        (SELECT flights.id
           FROM flights
           JOIN airports origin_airports ON flights.origin_airport_id = origin_airports.id
           JOIN airports destination_airports ON flights.destination_airport_id = destination_airports.id
          WHERE day = 29 AND month = 7 AND year = 2021
            AND origin_airports.city LIKE "Fiftyville" AND destination_airports.city NOT LIKE "Fiftyville"
          ORDER BY hour ASC, minute ASC
          LIMIT 1));

-- Intersects the previous queries.
SELECT *
  FROM people
 WHERE people.license_plate IN
     (SELECT license_plate
        FROM bakery_security_logs
       WHERE day = 28 AND month = 7 AND year = 2021
         AND hour = 10 AND minute BETWEEN 15 AND 25
         AND activity LIKE "exit")
INTERSECT
SELECT *
  FROM people
 WHERE people.id IN
    (SELECT person_id
       FROM bank_accounts
      WHERE bank_accounts.account_number IN
          (SELECT account_number
             FROM atm_transactions
            WHERE day = 28 AND month = 7 AND year = 2021
              AND atm_location LIKE "Leggett Street"
              AND transaction_type LIKE "withdraw"))
INTERSECT
SELECT *
  FROM people
 WHERE EXISTS
    (SELECT *
       FROM phone_calls
      WHERE day = 28 AND month = 7 AND year = 2021
        AND duration <= 60
        AND (caller = people.phone_number))
INTERSECT
SELECT *
  FROM people
 WHERE people.passport_number IN
    (SELECT passport_number
       FROM passengers
       JOIN flights ON passengers.flight_id = flights.id
       WHERE flights.id IN
        (SELECT flights.id
           FROM flights
           JOIN airports origin_airports ON flights.origin_airport_id = origin_airports.id
           JOIN airports destination_airports ON flights.destination_airport_id = destination_airports.id
          WHERE day = 29 AND month = 7 AND year = 2021
            AND origin_airports.city LIKE "Fiftyville" AND destination_airports.city NOT LIKE "Fiftyville"
          ORDER BY hour ASC, minute ASC
          LIMIT 1));
-- Bruce was busted on this Query!

-- Gets the name of the city the thief ESCAPED TO
SELECT destination_airports.city
  FROM flights
  JOIN airports origin_airports ON flights.origin_airport_id = origin_airports.id
  JOIN airports destination_airports ON flights.destination_airport_id = destination_airports.id
 WHERE day = 29 AND month = 7 AND year = 2021
   AND origin_airports.city LIKE "Fiftyville" AND destination_airports.city NOT LIKE "Fiftyville"
 ORDER BY hour ASC, minute ASC
 LIMIT 1;

-- Gets the people who Bruce called in that day.
 SELECT *
   FROM people
  WHERE people.phone_number IN
      (SELECT receiver
         FROM phone_calls
        WHERE day = 28 AND month = 7 AND year = 2021
          AND duration <= 60
          AND caller = "(367) 555-5533");
-- Robin was busted on this Query!