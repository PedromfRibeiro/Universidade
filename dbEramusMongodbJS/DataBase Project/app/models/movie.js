var mongoose     = require('mongoose');
var Schema       = mongoose.Schema;

var MovieSchema = new Schema({
	nome: String,
	date: String,
	genre: String,
});



module.exports = mongoose.model('Movie', MovieSchema);