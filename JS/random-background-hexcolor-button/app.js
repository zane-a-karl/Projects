var button = document.querySelector("button");
var body = document.querySelector("body");

body.style.backgroundColor = "gray";
var total_hex = 16*16;
button.onclick = function() {
    body.style.backgroundColor = "#" +
	(Math.random()*total_hex).toString(16) +
	(Math.random()*total_hex).toString(16) +
	(Math.random()*total_hex).toString(16);
}
