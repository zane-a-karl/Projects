struct Parser{
		sym: &str;
};

impl Parser {

		fn expression() {
				term();
				while(sym=='+'){
						next();
						term();
				}
		}

		fn term() {
				factor();
				while(sym=='*'){
						next();
						factor();
				}
		}
		
		fn factor() {
				if (sym=="("){
						next();
						expression();
						if(sym==")") {
								next();
						} else {
								error();
						}
				} else if (sym=='0'..'9'){
						next();
						while(sum=='0'..'9'){
								next();
						}
				} else {
						error();
				}
		}
}
