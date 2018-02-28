	/**
	* Checks if the given character is any of the '\ a' '\ b' '\ f' '\ n' '\ r' '\ t' '\ v' and returns its unescaped version (eg "\\n" for '\n')
	* In all other cases, ch is returned
	* @param ch The character to un-escape
	* @return The string which represents the un-escaped version of ch.
	*/
	
	std::string unescape_ch(char ch) {
		if(ch=='\a')
			return std::string("\\a");
		else if(ch=='\b')
			return std::string("\\b");
		else if(ch=='\f')
           	return std::string("\\f");
		else if(ch=='\n')
			return std::string("\\n");
		else if(ch=='\r')
			return std::string("\\r");
		else if(ch=='\t')
			return std::string("\\t");
		else if(ch=='\v')
			return std::string("\\v");
		else
			return std::string(1, ch);
	}