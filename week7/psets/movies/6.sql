SELECT AVG(rating) FROM ratings
    JOIN movies ON ratings.movie_id = movies.id
    WHERE year = 2012;

/*
SELECT AVG(rating) FROM ratings
    WHERE movie_id IN (
        SELECT id FROM movies
            WHERE year = 2012
    );
*/