Test route to make sure everything is working => http://localhost:27017/api
Create a user [POST] => http://localhost:27017/users

Route														                        HTTP 	    Description
http://localhost:27017/api/users							          GET       Get all the users.
http://localhost:27017/api/users							          POST			Create a user.
http://localhost:27017/api/users/id_user:users_id			  GET				Get a single user.
http://localhost:27017/api/users/id_user:users_id			  PUT				Update a user with new info.
http://localhost:27017/api/users/id_user:users_id			  DELETE 		Delete a user.
----------------------------------------------------------------------------------------------------------
http://localhost:27017/api/movies					              GET				 Get all the movies.
http://localhost:27017/api/movies					              POST			 Create a movie.
http://localhost:27017/api/movies/movies_id:movies_id		GET				 Get a single movie.
http://localhost:27017/api/movies/movies_id:movies_id		PUT				 Update a movie with new info.
http://localhost:27017/api/movies/movies_id:movies_id		DELETE 			 Delete a movie.
----------------------------------------------------------------------------------------------------------
http://localhost:27017/api/ratings					                GET				 Get all the ratings.
http://localhost:27017/api/ratings					                POST			 Create a rating.
http://localhost:27017/api/ratings/ratings_id:ratings_id		GET				 Get a single rating.
http://localhost:27017/api/ratings/ratings_id:ratings_id		PUT				 Update a rating with new info.
http://localhost:27017/api/ratings/ratings_id:ratings_id		DELETE 			 Delete a rating.
