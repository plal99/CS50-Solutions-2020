SELECT COUNT(title) FROM MOVIES 
JOIN ratings ON movies.id = ratings.movie_id 
WHERE ratings.rating = "10.0";