/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "kyim.h"
#include "Parse.h"
#include "emoticons.h"
#include <qregexp.h> 

extern KYahoo* main_window;

MsgParser::MsgParser()
  : fontString("font"), httpString("http"), sizeString("size"),
  faceString("face"), isFontDefaultForced(false)
{ 
  YahooTxtColor.insert(BLACK, "#000000");
  YahooTxtColor.insert(BLUE, "#0000FF");
  YahooTxtColor.insert(LIGHTBLUE, "#99A6EF");
  YahooTxtColor.insert(GRAY, "#C8C8C8");
  YahooTxtColor.insert(GREEN,"#097A1E");
  YahooTxtColor.insert(PINK, "#EF99E9");
  YahooTxtColor.insert(PURPLE, "#8A1BA3");
  YahooTxtColor.insert(ORANGE, "#FCC10F");
  YahooTxtColor.insert(RED, "#FF0000");
  YahooTxtColor.insert(OLIVE, "#546B50");

  // testing
  //  testParse();
}

void MsgParser::reset()
{
  lexPos = 0;
  pLexPos = -1;
  pLexType = LEXEOF;
  sym = LEXEOF;
  
  fontCount = 0;
  _boldCount = 0;
  _italicCount = 0;
  _underlineCount = 0;

  fontSizeStack.clear();
  colorNameStack.clear();
  fontNameStack.clear();

  outString.truncate(0);
}

bool MsgParser::formatMsg( QString& msg )
{
  reset();

  inString = msg;
  ParseMsg();
  
  msg = outString;
  parseIcons( msg );

  main_window->debugmsg("formatMsg: string: %s", (const char*)msg);
  
  return true;
}

bool MsgParser::formatReply( QString& msg )
{
  reset();

  inString = msg;
  ParseMsg();
  msg = outString;
  parseIcons( msg );

  main_window->debugmsg("formatReply: string: %s", (const char*)msg);
  
  return true;
}

//
// Strip all formatting and just return the plain text
//
bool MsgParser::formatLog( QString& msg )
{
  reset();

  inString = msg;
  ParseLog();
  msg = outString;

  return true;
}


bool MsgParser::ParseLog( )
{
  // filter out Yahoo URL tags,
  // the parser looks for the http:// instead
  inString.replace( QRegExp("\033\\[lm"), "" );
  inString.replace( QRegExp("\033\\[xlm"), "" );
  
  sym = lex();
  
  while ( sym != LEXEOF )
    {
      main_window->debugmsg("ParseLog: sym %d, lexBuf %s", 
			    sym, (const char*)lexBuf);

 	lexBufBkp = lexBuf;

      if ( fontStrip() )
	continue;

	if (sym == LEXEOF)  { outString += lexBufBkp+lexBuf; break; }	
 	lexBufBkp = lexBuf;

      if ( escStrip() )
	continue;

	if (sym == LEXEOF)  { outString += lexBufBkp+lexBuf; break; }	
 	lexBufBkp = lexBuf;

      if ( sym == NONPRINT )
	{
	  sym = lex();
	  continue;
	}

	if (sym == LEXEOF)  { outString += lexBufBkp+lexBuf; break; }	

      // just plain old text, append
      outString += lexBuf;
      sym = lex();
    }
  
  // lop off extra null character that seems to sneak into outString
  // it won't allow anything else to be appending, because of the null
  if ( (int)outString.constref(outString.length()-1) == 0 )
    outString.truncate( outString.length() - 1 );

  main_window->debugmsg("parse bad char -1 = %d char = %d", 
			(int)outString.constref(outString.length()-1),
			(int)outString.constref(outString.length()) );

  return true;
}

bool MsgParser::ParseMsg( )
{
  // filter out Yahoo URL tags,
  // the parser looks for the http:// instead
  inString.replace( QRegExp("\033\\[lm"), "" );
  inString.replace( QRegExp("\033\\[xlm"), "" );
 
  sym = lex();

  while ( sym != LEXEOF )
    {
      main_window->debugmsg("parse: sym %d, lexBuf %s", 
			    sym, (const char*)lexBuf);

	lexBufBkp = lexBuf;

      if ( httpParse() )
	continue;
	
	if (sym == LEXEOF)  { outString += lexBufBkp+lexBuf; break; }	
	lexBufBkp = lexBuf;
	
      if ( fontParse() )
	continue;

	if (sym == LEXEOF)  { outString += lexBufBkp+lexBuf; break; }	
	lexBufBkp = lexBuf;

      if ( sym == GREATERTHAN )
	{
	  outString += "&gt;";
		if (sym == LEXEOF)  { outString += lexBufBkp+lexBuf; break; }	
	  sym = lex();
	  continue;
	}

	if (sym == LEXEOF)  { outString += lexBufBkp+lexBuf; break; }	
	lexBufBkp = lexBuf;

      if ( sym == LESSTHAN )
	{
	  outString += "&lt;";
		if (sym == LEXEOF)  { outString += lexBufBkp+lexBuf; break; }	
	  sym = lex();
	  continue;
	}

	if (sym == LEXEOF)  { outString += lexBufBkp+lexBuf; break; }	
	lexBufBkp = lexBuf;

      if ( escParse() )
	continue;

      if ( sym == NEWLINE )
	{
	  outString += "<br>";
		if (sym == LEXEOF)  { break; }	
	  sym = lex();
	  continue;
	}

	if (sym == LEXEOF)  { outString += lexBufBkp+lexBuf; break; }	
	lexBufBkp = lexBuf;

      if ( sym == NONPRINT )
	{
		if (sym == LEXEOF)  { break; }	
	  sym = lex();
	  continue;
	}

	if (sym == LEXEOF)  { outString += lexBufBkp+lexBuf; break; }	

      // just plain old text, append
      outString += lexBuf;
      sym = lex();
    }

  // lop off extra null character that seems to sneak into outString
  // it won't allow anything else to be appending, because of the null
  if ( (int)outString.constref(outString.length()-1) == 0 )
    outString.truncate( outString.length() - 1 );

  main_window->debugmsg("parse bad char -1 = %d char = %d", 
			(int)outString.constref(outString.length()-1),
			(int)outString.constref(outString.length()) );

  while ( fontCount-- > 0 )
    outString += "</font>";

  while ( _boldCount-- > 0 )
    outString += "</b>";

  while ( _italicCount-- > 0 )
    outString += "</i>";

  while ( _underlineCount-- > 0 )
    outString += "</u>";

  outString += "<br>";

  return true;
}


bool MsgParser::httpParse()
{
  if ( sym == HTTP )
    {
      sym = lex();
    }
  else
    {
      main_window->debugmsg("httpParse sym %d lexBuf %s, returning false", 
			    sym, (const char*) lexBuf );
      return false;
    }

  if ( sym == COLON )
    {
      sym = lex();
    }
  else
    {
      backupLex();
      return false;
    }

  if ( sym == SLASH )
    {
      sym = lex();
      if ( sym == SLASH )
	{
	  sym = lex();
	  
	  if ( urlParse() )
	    return true;
	}
    }

  return false;
}

bool MsgParser::urlParse( void )
{
  QString urlString;
  
  main_window->debugmsg("urlParse: sym %d", sym);
  while ( (sym != LEXEOF) && (sym != SPACE) && (sym != NONPRINT) && (sym != NEWLINE) )
    {
      urlString += lexBuf;
      sym = lex();      
      main_window->debugmsg("urlParse: sym %d lexBuf %s", sym, (const char*)lexBuf);
    }
  if (sym == LEXEOF) urlString=urlString.left(urlString.length()-1);
  outString += QString("<a href=\"http://");
  outString += urlString;
  outString += QString("\">");
  outString += "http://";
  outString += urlString;
  outString += QString("</a>");

  return true;
  
}

bool MsgParser::fontParse( )
{
  QString fname;
  
  if ( sym == LESSTHAN )
    {
      sym = lex();
    }
  else
    return false;

  if ( sym == FONT )
    {
      sym = lex();
    }
  else
    {
      main_window->debugmsg("fontParse, sym %d, backup lex", sym);
      backupLex();
      return false;
    }
  
  if ( fontNameParse( fname) ) 
    {
      if ( isFontDefaultForced == false ) 
	{
	  if ( fontCount > 0 )
	    {
	      fontCount--;
	      outString += "</font>";
	    }
	  outString += "<font ";
	  if ( !colorNameStack.isEmpty() )
	    {
	      outString += "color=\"";
	      outString += colorNameStack.last();
	      outString += "\" ";
	    }
	  
	  outString += fname;
	  outString += ">";
	  fontCount++;
	}
      return true;
    }
  else if ( sizeParse( fname ) )
    {
      if ( isFontDefaultForced == false ) 
	{
	  if ( fontCount > 0 )
	    {
	      fontCount--;
	      outString += "</font>";
	    }
	  outString += "<font ";
	  
	  if ( !colorNameStack.isEmpty() )
	    { 
	      outString += "color=\"";
	      outString += colorNameStack.last();
	      outString += "\" ";
	    }
	  
	  outString += fname;
	  outString += ">";
	  fontCount++;
	}
      return true;
    }

  return false;
}

// called from font parse .. don't use externally
// looks for
// ' face="....."' (maybe size="..") then '>'
bool MsgParser::fontNameParse( QString& fname )
{
  
  if ( sym == SPACE )
    {
      sym = lex();
    }
     
  if ( sym == FACE )
    {
      sym = lex();
    }

  if ( sym == EQUAL )
    {
      sym = lex();
    }
  
  if ( sym == QUOTE )
    {
      sym = lex();
		  
      fname += "face=\"";
      while ( sym != QUOTE )
	{
	  fname += lexBuf;
	  sym = lex();
	}
      fname += "\" ";
      
      // advance past quote
      sym = lex();
      
      if ( sizeParse( fname ) )
	return true;
      else
	{
	  while ( sym != GREATERTHAN )
	    {
	      sym = lex();
	    }
	  sym = lex();
	  return true;
	}
    }

  return false;
}

// called from font parse .. don't use externally
//
// looks for
// ' size="...">'
bool MsgParser::sizeParse( QString& fname )
{
  if ( sym == SPACE) 
    {
      sym = lex();
    }
  
  if (sym == SIZE) 
    {
      sym = lex();
    }
      
  if ( sym == EQUAL )
    {
      sym = lex();
    }
  
  if ( sym == QUOTE )
    {
      sym = lex();
    } 
     
  if ( sym == NUMBER )
    {
      int size = 0;
      size = lexBuf.toInt();

      // convert point size to QStyleSheet logical font numbers
      if ( size < 7 )
	size = 1;
      else if ( size < 8 )
	size = 2;
      else if ( size < 9 )
	size = 3;
      else if ( size < 10 )
	size = 4;
      else if ( size < 14 )
	size = 5;
      else if ( size < 24 )
	size = 6;
      else 
	size = 7;


      fontSizeStack.push(size); // save current font size for color parsing
      main_window->debugmsg("size parse, size = %d", size);

      fname += "size=\"";
      fname += QString::number(size);
      fname += "\"";

      sym = lex();
      
      while ( sym != GREATERTHAN )
	{
	  sym = lex();
	}
      sym = lex();
      return true;
    }

  return false;

}


bool MsgParser::escParse( )
{

  if ( sym == ESC )
    {
      sym = lex();
    }
  else
    return false;

  if ( sym == LBRACKET )
    {
      sym = lex();
    }
  else
    {
      backupLex();
      return false;
    }

  if ( sym == NUMBER )
    {
      int lexNum =  lexBuf.toInt();
      if ( (lexNum >= 30) && (lexNum < 40 ) )
	{
	  if ( isFontDefaultForced == false ) 
	    {
	      // its a color
	      outString += "<font ";
	      
	      if ( !fontSizeStack.isEmpty() )
		{
		  int size = fontSizeStack.last();
		  outString += "size=\"";
		  outString += QString::number(size);
		  outString += "\" ";
		}
	      if ( !fontNameStack.isEmpty() )
		{ 
		  outString += "name=\"";
		  outString += fontNameStack.last();
		  outString += "\" ";
		}
	      
	      fontCount++;
	      colorNameStack.push(  YahooTxtColor.at(lexNum-30) );
	      outString += "color=\"";
	      outString += YahooTxtColor.at(lexNum-30);
	      outString += "\">";
	    }
	}
      else
	{
	  if ( isFontDefaultForced == false ) 
	    {
	      // its a font attribute
	      switch( lexNum )
		{
		case 1:
		  outString += "<b>";
		  _boldCount++;
		  break;
		case 2:
		  outString += "<i>";
		  _italicCount++;
		  break;
		case 4:
		  outString += "<u>";
		  _underlineCount++;
		  break;
		default:
		  break;	      
		}
	    }
	}

      sym = lex();
      if ( (sym == WORD) || (sym == FONT) 
	   || (sym == HTTP) || (sym == SIZE) 
	   || (sym == FACE) )
	{
	  if ( lexBuf.length() == 1 )
	    {
	      // just the 'm' in next lex symbol
	      sym = lex();
	      return true;
	    }
	  else
	    {
	      // take off first 'm' and
		  lexBuf.remove( 0, 1 );
		  return true;
	    }
	}
    }
  else if ( sym == WORD )
    {
      if ( (lexBuf.length() == 1) && ( lexBuf[0] == 'x' ) )
	{
	  sym = lex();
	  if ( sym == NUMBER )
	    {
	      if ( isFontDefaultForced == false ) 
		{
		  int lexNum =  lexBuf.toInt();
		  switch( lexNum )
		    {
		    case 1:
		      outString += "</b>";
		      _boldCount--;
		      break;
		    case 2:
		      outString += "</i>";
		      _italicCount--;
		      break;
		    case 4:
		      outString += "</u>";
		      _underlineCount--;
		      break;
		    default:
		      break;	      
		    }
		}

	      sym = lex();
	      if ( (sym == WORD) || (sym == FONT) 
		   || (sym == HTTP) || (sym == SIZE) 
		   || (sym == FACE) )
		{
		  if ( lexBuf.length() == 1 )
		    {
		      // just	 the 'm' in next lex symbol
		      sym = lex();
		      return true;
		    }
		  else
		    {
		      // take off first 'm' and
		      // trick the main loop into keep looking at this symbol
		      lexBuf.remove( 0, 1 );
		      return true;
		    }
		}
	    }
	}
    }
 
  return false;
}


bool MsgParser::fontStrip( void )
{
  QString fname;
  
  if ( sym == LESSTHAN )
    {
      sym = lex();
    }
  else
    return false;

  if ( sym == FONT )
    {
      sym = lex();
    }
  else
    {
      main_window->debugmsg("fontParse, sym %d, backup lex", sym);
      backupLex();
      return false;
    }

  if ( fontNameParse( fname) ) 
    {
      return true;
    }
  else if ( sizeParse( fname ) )
    {
      return true;
    }

  return false;
}

bool MsgParser::escStrip( void )
{

  if ( sym == ESC )
    {
      sym = lex();
    }
  else
    return false;

  if ( sym == LBRACKET )
    {
      sym = lex();
    }
  else
    {
      backupLex();
      return false;
    }

  if ( sym == NUMBER )
    {

      sym = lex();
      if ( (sym == WORD) || (sym == FONT) 
	   || (sym == HTTP) || (sym == SIZE) 
	   || (sym == FACE) )
	{
	  if ( lexBuf.length() == 1 )
	    {
	      // just the 'm' in next lex symbol
	      sym = lex();
	      return true;
	    }
	  else
	    {
	      // take off first 'm' and
		  lexBuf.remove( 0, 1 );
		  return true;
	    }
	}
    }
  else if ( sym == WORD )
    {
      if ( (lexBuf.length() == 1) && ( lexBuf[0] == 'x' ) )
	{
	  sym = lex();
	  if ( sym == NUMBER )
	    {
	      sym = lex();
	      if ( (sym == WORD) || (sym == FONT) 
		   || (sym == HTTP) || (sym == SIZE) 
		   || (sym == FACE) )
		{
		  if ( lexBuf.length() == 1 )
		    {
		      // just	 the 'm' in next lex symbol
		      sym = lex();
		      return true;
		    }
		  else
		    {
		      // take off first 'm' and
		      // trick the main loop into keep looking at this symbol
		      lexBuf.remove( 0, 1 );
		      return true;
		    }
		}
	    }
	}
    }
 
  return false;

}



// restores the previous lex Position, to back up one position
// should implement as a stack later
void MsgParser::backupLex()
{
  if ( pLexPos != -1 )
    {
      lexPos = pLexPos;
      sym = pLexType;
      lexBuf = pLexBuf;
    }
}

// check that single character qualifies as WORD type
lexTypes MsgParser::checkChar( int pos )
{
  // single character matches
  switch ( ((char)lexBuf.constref(pos)) )
    {
    case ' ':
      return ( SPACE );
      break;
    case '"':
      return ( QUOTE );
      break;
    case '=':
      return ( EQUAL ); 
      break;
    case '/':
      return ( SLASH ); 
      break;
    case '<':
      return ( LESSTHAN );
      break;
    case '>':
      return ( GREATERTHAN );
      break;
    case ':':
      return ( COLON );
      break;
    case '.':
      return ( DOT );
      break;
    case '\033':
      return ( ESC );
      break;
    case '[':
      return ( LBRACKET );
      break;
    case '\n':
      return ( NEWLINE );
      break;
    }
  
  if ( !(lexBuf[pos].isLetter()) && lexBuf[pos].isPrint() )
    return ( SYMBOL );
  
  // catch the ACK's yahoo sprinkle's everywhere
  if ( !lexBuf[pos].isPrint() )
    return ( NONPRINT );
  
  return ( WORD );
}

lexTypes MsgParser::lex( )
{
  int wordPos = 0;

  // save last lex starting pointer
  pLexPos = lexPos;
  pLexBuf = lexBuf;
  pLexType = sym;

  // clear out previous parsed word
  lexBuf.truncate(0);

  if ( lexPos == (int)inString.length() )
    {
      lexPos = 0;
      pLexPos = -1;
      return ( LEXEOF );
    }

  lexBuf += inString[lexPos++];
  wordPos++;
  
  if ( !(lexBuf[0].isNumber()) )
    {
      while ( lexPos <= (int)inString.length() )
	{
	  // whole word match
	  if ( -1 != lexBuf.find(fontString) )
	    return ( FONT );
	  else if ( -1 != lexBuf.find(httpString) )
	    return ( HTTP );
	  else if ( -1 != lexBuf.find(sizeString) )
	    return ( SIZE );
	  else if ( -1 != lexBuf.find(faceString) )
	    return ( FACE );

	  lexTypes tmpType = checkChar(wordPos-1);
	  
	  // not a letter, need to back up and return value
	  // (next call to lex will pick up the new character)
	  if ( tmpType != WORD )
	    {
	      if ( wordPos != 1 )
		{
		  lexBuf.remove(wordPos-1, 1);
		  lexPos--;
		  main_window->debugmsg("lex: word #1");
		  return ( WORD );
		}
	      else 
		return ( tmpType );
	    }
       
	  lexBuf += inString[lexPos++];
	  wordPos++;
	} 
      if ( wordPos > 1 )
	return ( WORD );
    }
  else
    {
      while ( lexPos <= (int)inString.length() )
	{
	  // not a number, need to back up and return value
	  // (next call to lex will pick up the new character)
	  if ( !(lexBuf[wordPos-1].isNumber()) )
	    {
	      lexBuf.remove(wordPos-1, 1);
	      lexPos--;
	      return ( NUMBER );
	    }

	  lexBuf += inString[lexPos++];
	  wordPos++;
	}
      if ( wordPos > 1 )
	return ( WORD );
    }

  lexPos = 0;
  pLexPos = -1;
  return ( LEXEOF );
}

const char* MsgParser::lex2string( lexTypes t )
{  
  const char * lexString[] = {
    "word",
    "num", 
    "symb",
    "space",
    "esc",
    "font",
    "http",
    "size",
    "=",
    "/",
    "\"",
    "<",
    ">",
    ":",
    "EOF",
    "face",
    "."
  };

  return lexString[t];
}


/**
 * Scan for emoticons and replace them with <img source=...> tag.
 *
 * @param text QString reference of message to scan.
 */
void MsgParser::parseIcons( QString& msg )
{
 #define FACE(x) \
    QString("<img src=\"" + QString(DATADIR) + "/pics/emoticon_" + QString(x) + ".png\">")

  if ( main_window->options.use_emoticons == false )
    return;

  if (msg.contains( STR_CLOWN1 ) )
  {
    msg.replace( QRegExp( STR_RE_CLOWN1), FACE("clown"));
  }

  if (msg.contains( STR_CLOWN2 ) )
  {
    msg.replace( QRegExp( STR_RE_CLOWN2), FACE("clown"));
  }
  if (msg.contains( STR_ANGEL1))
  {
    msg.replace( QRegExp(STR_RE_ANGEL1),FACE("angel"));
  }  
  if (msg.contains( STR_ANGEL2))
  {
    msg.replace( QRegExp(STR_RE_ANGEL2),FACE("angel"));
  }
  if (msg.contains( STR_DEVIL1 ) )
  {
    msg.replace( QRegExp( STR_RE_DEVIL1), FACE("devil"));
  }
  if (msg.contains( STR_DEVIL2 ) )
  {
    msg.replace( QRegExp( STR_RE_DEVIL2), FACE("devil"));
  }
  if ( msg.contains( STR_LAUGH1 ) )
  {
    msg.replace( QRegExp( STR_RE_LAUGH1), FACE("laugh"));
  }
  if (msg.contains( STR_LAUGH2 ) )
  {
    msg.replace( QRegExp( STR_RE_LAUGH2), FACE("laugh"));
  }
  if ( msg.contains( STR_SMILEY1 ) )
  {
    msg.replace( QRegExp( STR_RE_SMILEY1), FACE("smile"));
  }
  if (msg.contains( STR_SMILEY2 ) )
  {
    msg.replace( QRegExp( STR_RE_SMILEY2), FACE("smile"));
  }
  if (msg.contains( STR_TONGUE1 ) )
  {
    msg.replace( QRegExp( STR_TONGUE1), FACE("tongue"));
  }
  if (msg.contains( STR_TONGUE2 ) )
  {
    msg.replace( QRegExp( STR_TONGUE2), FACE("tongue"));
  }
  if (msg.contains( STR_TONGUE3 ) )
  {
    msg.replace( QRegExp( STR_TONGUE3), FACE("tongue"));
  }
  if (msg.contains( STR_TONGUE4 ) )
  {
    msg.replace( QRegExp( STR_TONGUE4), FACE("tongue"));
  }
  if (msg.contains( STR_BLINK1 ) )
  {
    msg.replace( QRegExp( STR_RE_BLINK1), FACE("blink"));
  }
  if (msg.contains( STR_BLINK2 ) )
  {
    msg.replace( QRegExp( STR_RE_BLINK2), FACE("blink"));
  }
  if (msg.contains( STR_CRY1 ) )
  {
    msg.replace( QRegExp( STR_RE_CRY1), FACE("cry"));
  }
  if (msg.contains( STR_CRY2 ) )
  {
    msg.replace( QRegExp( STR_RE_CRY2), FACE("cry"));
  }
  if (msg.contains( STR_SAD1 ) )
  {
    msg.replace( QRegExp( STR_RE_SAD1), FACE("sad"));
  }
  if (msg.contains( STR_SAD2 ) )
  {
    msg.replace( QRegExp( STR_RE_SAD2), FACE("sad"));
  }

  if (msg.contains( STR_SURP1 ) )
  {
    msg.replace( QRegExp( STR_SURP1), FACE("surp"));
  }
  if (msg.contains( STR_SURP2 ) )
  {
    msg.replace( QRegExp( STR_SURP2), FACE("surp"));
  }
  if (msg.contains( STR_ANGRY1 ) )
  {
    msg.replace( QRegExp( STR_RE_ANGRY1), FACE("angry"));
  }
  if (msg.contains( STR_ANGRY2 ) )
  {
    msg.replace( QRegExp( STR_RE_ANGRY2), FACE("angry"));
  }
  if (msg.contains( STR_BIGSM1 ) )
  {
    msg.replace( QRegExp( STR_BIGSM1), FACE("bigsm"));
  }
  if (msg.contains( STR_BIGSM2 ) )
  {
    msg.replace( QRegExp( STR_BIGSM2), FACE("bigsm"));
  }
  if (msg.contains( STR_GLASS1 ) )
  {
    msg.replace( QRegExp( STR_RE_GLASS1), FACE("glass"));
  }
  if (msg.contains( STR_GLASS2 ) )
  {
    msg.replace( QRegExp( STR_RE_GLASS2), FACE("glass"));
  }
  if (msg.contains( STR_GLASS3 ) )
  {
    msg.replace( QRegExp( STR_GLASS3), FACE("glass"));
  }
  if (msg.contains( STR_SUNGL ) )
  {
    msg.replace( QRegExp( STR_RE_SUNGL), FACE("sungl"));
  }
  if (msg.contains( STR_SARC1 ) )
  {
    msg.replace( QRegExp( STR_SARC1), FACE("sarc"));
  }
  if (msg.contains( STR_SARC2 ) )
  {
    msg.replace( QRegExp( STR_SARC2), FACE("sarc"));
  }
  if (msg.contains( STR_QUEST1 ) )
  {
    msg.replace( QRegExp( STR_QUEST1), FACE("quest"));
  }
/*  if (msg.contains( STR_QUEST2 ) )
  {
    msg.replace( QRegExp( STR_QUEST2), FACE("quest"));
  }*/
  if (msg.contains( STR_LOVE1 ) )
  {
    msg.replace( QRegExp( STR_LOVE1), FACE("love"));
  }
  if (msg.contains( STR_LOVE2 ) )
  {
    msg.replace( QRegExp( STR_LOVE2), FACE("love"));
  }
  if (msg.contains( STR_LOVE3 ) )
  {
    msg.replace( QRegExp( STR_LOVE3), FACE("love"));
  }
  if (msg.contains( STR_LOVE4 ) )
  {
    msg.replace( QRegExp( STR_LOVE4), FACE("love"));
  }
  if (msg.contains( STR_QUIET1 ) )
  {
    msg.replace( QRegExp( STR_RE_QUIET1), FACE("quiet"));
  }
  if (msg.contains( STR_QUIET2 ) )
  {
    msg.replace( QRegExp( STR_RE_QUIET2), FACE("quiet"));
  }
  if (msg.contains( STR_STRAIGHT1 ) )
  {
    msg.replace( QRegExp( STR_RE_STRAIGHT1), FACE("straight"));
  }
  
  if (msg.contains( STR_STRAIGHT2 ) )
  {
    msg.replace( QRegExp( STR_RE_STRAIGHT2), FACE("straight"));
  }

  if (msg.contains( STR_BLUSH1 ) )
  {
    msg.replace( QRegExp( STR_BLUSH1), FACE("blush"));
  }
  if (msg.contains( STR_SLEEP1 ) )
  {
    msg.replace( QRegExp( STR_RE_SLEEP1), FACE("sleep"));
  }
  if (msg.contains( STR_SLEEP2 ) )
  {
    msg.replace( QRegExp( STR_RE_SLEEP2), FACE("sleep"));
  }
  if (msg.contains( STR_BYE1 ) )
  {
    msg.replace( QRegExp( STR_BYE1), FACE("bye"));
  }
  if (msg.contains( STR_PISSED1 ) )
  {
    msg.replace( QRegExp( STR_RE_PISSED1), FACE("pissed"));
  }
  if (msg.contains( STR_YUCK1 ) )
  {
    msg.replace( QRegExp( STR_YUCK1), FACE("yuck"));
  }
  if (msg.contains( STR_KISS1 ) )
  {
    msg.replace( QRegExp( STR_RE_KISS1), FACE("kiss"));
  }
  if (msg.contains( STR_KISS1 ) )
  {
    msg.replace( QRegExp( STR_RE_KISS1), FACE("kiss"));
  }
}

/**
   Unit test for parser. Use this to verify output from parser is correct.

   Add more test cases as bugs appear.
   */
void MsgParser::testParse( void )
{
  // testParse: colors1 = <font color="#000000">black <font color="#0000FF">blue <font color="#99A6EF">lightblue <font color="#C8C8C8">grey <font color="#097A1E">green<br/>
  QString colors1 = "\033[30mblack \033[31mblue \033[32mlightblue \033[33mgrey \033[34mgreen";
  formatMsg(colors1);
  main_window->debugmsg("testParse: colors1 = %s", (const char*)colors1 );

  // testParse: colors2 = <font color="#EF99E9">pink <font color="#8A1BA3">purple <font color="#FCC10F">orange <font color="#FF0000">red <font color="#546B50">olive<br/>

  QString colors2 = "\033[35mpink \033[36mpurple \033[37morange \033[38mred \033[39molive";
  formatMsg(colors2);
  main_window->debugmsg("testParse: colors2 = %s", (const char*)colors2 );

  // testParse: bold1 = <b>bold on</b> off <b>on</b> <br/>
  QString bold1 = "\033[1mbold on\033[x1m off \033[1mon\033[x1m off";
  formatMsg(bold1);
  main_window->debugmsg("testParse: bold1 = %s", (const char*)bold1 );

}
