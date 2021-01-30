/* w3color.js ver.1.18 by w3schools.com (Do not remove this line)*/
(function () {
    function w3color(color) {
	if (!(this instanceof w3color)) {
	    return new w3color(color);
	}
	this.attachValues(toColorObject(color));
    }
    w3color.prototype = {
	toRgbString : function () {
	    return "rgb(" + this.red + ", " + this.green + ", " + this.blue + ")";
	},
	toHexString : function () {
	    var r = toHex(this.red);
	    var g = toHex(this.green);
	    var b = toHex(this.blue);
	    return "#" +  r + g + b;
	},
	isDark : function (n) {
	    var m = (n || 128);
	    return (((this.red * 299 + this.green * 587 + this.blue * 114) / 1000) < m);
	},
	attachValues : function(color) {
	    this.red = color.red;
	    this.green = color.green;
	    this.blue = color.blue;
	    this.valid = color.valid;
	}
    };
    function toColorObject(c) {
	var x, y, typ, arr = [], arrlength, i, match, rgb;
	c = w3trim(c.toLowerCase());
	x = c.substr(0,1).toUpperCase();
	y = c.substr(1);
	if (c.substr(0, 3) == "rgb") {
	    typ = c.substr(0, 3);
	    c = c.substr(3);
	    arrlength = 3;
	    c = c.replace("(", "");
	    c = c.replace(")", "");
	    arr = c.split(",");
	    if (typ == "rgb") {
		if (arr.length != arrlength) {
		    return emptyObject();
		}
		for (i = 0; i < arrlength; i++) {
		    if (arr[i] == "" || arr[i] == " ") {arr[i] = "0"; }
		    if (arr[i].indexOf("%") > -1) {
			arr[i] = arr[i].replace("%", "");
			arr[i] = Number(arr[i] / 100);
			if (i < 3 ) {arr[i] = Math.round(arr[i] * 255);}
		    }
		    if (isNaN(arr[i])) {return emptyObject(); }
		    if (parseInt(arr[i]) > 255) {arr[i] = 255; }
		    if (i < 3) {arr[i] = parseInt(arr[i]);}
		    if (i == 3 && Number(arr[i]) > 1) {arr[i] = 1;}
		}
		rgb = {r : arr[0], g : arr[1], b : arr[2]};
	    }
	}
	return colorObject(rgb);
    }
    function colorObject(rgb) {
	var color;
	if (!rgb) {return emptyObject();}
	color = {
	    red : rgb.r,
	    green : rgb.g,
	    blue : rgb.b,
	    valid : true
	};
	color = roundDecimals(color);
	return color;
    }
    function emptyObject() {
	return {
	    red : 0,
	    green : 0,
	    blue : 0,
	    valid : false
	};
    }
    function roundDecimals(c) {
	c.red = Number(c.red.toFixed(0));
	c.green = Number(c.green.toFixed(0));
	c.blue = Number(c.blue.toFixed(0));
	return c;
    }
    function toHex(n) {
	var hex = n.toString(16);
	while (hex.length < 2) {hex = "0" + hex; }
	return hex;
    }
    function w3trim(x) {
	return x.replace(/^\s+|\s+$/g, '');
    }
    window.w3color = w3color;

})();
