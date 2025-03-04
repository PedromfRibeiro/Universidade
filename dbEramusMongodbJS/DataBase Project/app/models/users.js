var mongoose     = require('mongoose');
var Schema       = mongoose.Schema;

var UserSchema   = new Schema({
	name: String,
	nick: String,
	birthdate:String
});

module.exports = mongoose.model('user', UserSchema);