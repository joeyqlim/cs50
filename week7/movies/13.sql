SELECT name FROM people
WHERE people.id IN
    (SELECT person_id FROM stars JOIN movies ON stars.movie_id=movies.id
    WHERE movie_id IN
        (SELECT movie_id FROM movies JOIN stars ON movies.id=stars.movie_id 
        JOIN people ON people.id=stars.person_id
        WHERE people.id IN
            (SELECT id FROM people
            WHERE name="Kevin Bacon" AND birth=1958
            )
        )
    )
EXCEPT SELECT name FROM people where name="Kevin Bacon" AND birth=1958

