var mongoose     = require('mongoose');
var Schema       = mongoose.Schema;

var RatingsSchema = new Schema({
	idMovie: String,
	idUser: String,
	rating:Number,
});

module.exports = mongoose.model('Ratings', RatingsSchema);