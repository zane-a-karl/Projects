var slider = document.querySelector(".slider");

var output = document.querySelector(".demo");
output.innerHTML = slider.value;
var body = document.querySelector("body");

slider.oninput = function() {
    let v = this.value;
    let step = 255;
    output.innerHTML = v;
    changeHex(v);
}

// https://www.w3schools.com/css/css_colors_hex.asp was super helpful.
function changeHex(v) {
    var col, r, g, b;
    let step = 255;
    let phase = Math.floor(v/step);
    switch (phase) {
    case 0:
	r = step;
	g = (v % step);
	b = 0;
	break;
    case 1:
	r = step - (v % step);
	g = step;
	b = 0;
	break;
    case 2:
	r = 0;
	g = step;
	b = (v % step);
	break;
    case 3:
	r = 0;
	g = step - (v % step);
	b = step;
	break;
    default:
	r = step;
	g = step;
	b = step;
	break;
    }
    col = w3color("rgb(" + r + "," + g + "," + b + ")");
    body.style.background = col.toRgbString();
    document.querySelector('.hexresulttext').innerHTML = col.toHexString();
    if (col.isDark(150)) {
	document.querySelector('.hexresulttext').style.color = "#ffffff";
    } else {
	document.querySelector('.hexresulttext').style.color = "#1f2d3d";
    }
}

