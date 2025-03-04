// BASE SETUP
// =============================================================================
var express    = require('express');
var bodyParser = require('body-parser');
var app        = express();
var morgan     = require('morgan');

app.use(morgan('dev'));
app.use(bodyParser.urlencoded({ extended: true }));
app.use(bodyParser.json());

var port     = process.env.PORT || 27017; // set our port for statics 

// DATABASE SETUP
var mongoose   = require('mongoose');
mongoose.connect('mongodb://127.0.0.1:27017',{useNewUrlParser:true}); // connect to localhost database

mongoose.connection  
.once("open",()=> console.log('!!Connected'))
.on("error",error => console.log("!!your error",error));

var user     = require('./app/models/users.js');
var movie    = require('./app/models/movie.js');
var ratings  = require('./app/models/ratings.js');


//user.collection.insertOne({name:"Pedro Ribeiro",nick:"pribeiro",birthdate:"20-06-1997"});
//movie.collection.insertOne({name:"Toy Story ",date:"20-06-1997",genre:"Animation"});
//ratings.collection.insertOne({ idMovie: '1',idUser:"1" ,rating:"3" });


// ROUTES FOR OUR API
// =============================================================================

var router = express.Router();
router.get('/', function(req, res) {
	res.json({ message: 'Welcome to our Rest API service!' });	
});

// on routes that end in /users
// ----------------------------------------------------
router.route('/users/')
	.post(function(req, res) {
		var usr = new user();		// create a new instance of the user model
		usr.name = req.body.name;  // set the users name (comes from the request)
		usr.nick = req.body.nick;
		usr.birthdate = req.body.birthdate;

		usr.save(function(err) {
			if (err) res.send(err);
			res.json({ message: 'user created!' });
		});
	})

	// get all the users (accessed at GET http://localhost:27017/api/users)
	.get(function(req, res) {
		user.find(function(err, users) {
			if (err) res.send(err);
			res.json(users);
		});
	});
// on routes that end in /users/:user_id
// ----------------------------------------------------
router.route('/users/id_user::user_id')
	 //get the user with that id
	.get(function(req, res) {
		user.findById(req.params.user_id, function(err, user) {
			if (err) res.send(err);
			res.json(user);
		});
	})

	// update the user with this id
	.put(function(req, res) {
		user.findById(req.params.user_id, function(err, user) {
			if (err) res.send(err);
			user.name = req.body.name;  // set the users name (comes from the request)
			user.nick = req.body.nick;
			user.birthdate = req.body.birthdate;
			user.save(function(err) {
				if (err) res.send(err);
				res.json({ message: 'user updated!' });
			});
		});
	})

	// delete the user with this id
	.delete(function(req, res) {
		user.remove({
			_id: req.params.user_id
		}, function(err, user) {
			if (err) res.send(err);
			res.json({ message: 'Successfully deleted' });
		});
	});


// on routes that end in /movies
// ----------------------------------------------------
router.route('/movies/')
	.post(function(req, res) {
		var mov = new movie();		// create a new instance of the movie model
		mov.nome = req.body.nome;  // set the movies name (comes from the request)
		mov.date = req.body.date;
		mov.genre = req.body.genre;

		mov.save(function(err) {
			if (err) res.send(err);
			res.json({ message: 'movie created!' });
		});
	})

	// get all the movies (accessed at GET http://localhost:27017/api/movies)
	.get(function(req, res) {
		movie.find(function(err, movies) {
			if (err) res.send(err);
			res.json(movies);
		});
	});
// on routes that end in /movies/:movies_id
// ----------------------------------------------------
router.route('/movies/movies_id::movies_id')
	 //get the movie with that id
	.get(function(req, res) {
		movie.findById(req.params.movies_id, function(err, movie) {
			if (err) res.send(err);
			res.json(movie);
		});
	})

	// update the movie with this id
	.put(function(req, res) {
		movie.findById(req.params.movies_id, function(err, movie) {
			if (err) res.send(err);
				
			movie.nome = req.body.nome;  // set the movies name (comes from the request)
			movie.date = req.body.date;
			movie.genre = req.body.genre;

					movie.save(function(err) {
				if (err) res.send(err);
				res.json({ message: 'movie updated!' });
			});
		});
	})

	// delete the movie with this id
	.delete(function(req, res) {
		movie.remove({
			_id: req.params.movies_id
		}, function(err, movie) {
			if (err) res.send(err);
			res.json({ message: 'Successfully deleted' });
		});
	});


// on routes that end in /ratings
// ----------------------------------------------------


router.route('/ratings/')
	.post(function(req, res) {
		var rate = new ratings();		// create a new instance of the rating model
		rate.idMovie = req.body.idMovie;  // set the ratings name (comes from the request)
		rate.idUser = req.body.idUser;
		rate.rating = req.body.rating;

		rate.save(function(err) {
			if (err) res.send(err);
			res.json({ message: 'rating created!' });
		});
	})

	// get all the ratings (accessed at GET http://localhost:27017/api/ratings)
	.get(function(req, res) {
		ratings.find(function(err, ratings) {
			if (err) res.send(err);
			res.json(ratings);
		});
	});
// on routes that end in /ratings/:rating_id
// ----------------------------------------------------
router.route('/ratings/ratings_id::rating_id')
	 //get the rating with that id
	.get(function(req, res) {
		ratings.findById(req.params.rating_id, function(err, rating) {
			if (err) res.send(err);
			res.json(rating);
		});
	})

	// update the rating with this id
	.put(function(req, res) {
		ratings.findById(req.params.rating_id, function(err, rating) {
			if (err) res.send(err);
			rating.idMovie = req.body.idMovie;  // set the ratings name (comes from the request)
			rating.idUser = req.body.idUser;
			rating.rating = req.body.rating;
			rating.save(function(err) {
				if (err) res.send(err);
				res.json({ message: 'rating updated!' });
			});
		});
	})

	// delete the rating with this id
	.delete(function(req, res) {
		ratings.remove({
			_id: req.params.rating_id
		}, function(err, rating) {
			if (err) res.send(err);
			res.json({ message: 'Successfully deleted' });
		});
	});

// REGISTER OUR ROUTES -------------------------------
app.use('/api', router);

// START THE SERVER
// =============================================================================
app.listen(port);
console.log('Magic happens on port ' + port);
