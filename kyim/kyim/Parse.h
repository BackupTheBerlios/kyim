/* *************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PARSE_H
#define PARSE_H

#include <qvaluestack.h>
#include <qstring.h>
#include <qstrlist.h>

/**
   lexTypes
   The symbol list the lexical analyizer will return.
*/
enum types
{	
  WORD = 0,
  NUMBER = 1,
  SYMBOL = 2,
  SPACE = 3,
  ESC = 4,
  FONT = 5,
  HTTP = 6,
  SIZE = 7,
  EQUAL = 8,
  SLASH = 9,
  QUOTE = 10,
  LESSTHAN = 11,
  GREATERTHAN = 12,
  COLON = 13,
  LEXEOF = 14,
  FACE = 15,
  DOT = 16,
  LBRACKET = 17,
  NEWLINE = 18,
  NONPRINT = 19
}; 
typedef enum types lexTypes;

class MsgParser
{
 public:
  /**
     Formats chat messages to use html-like tags for the chat window dialogs.
     
     @param text QString reference of message that needs to be formatted. 
  */
  MsgParser();
  bool formatMsg( QString& msg );
  bool formatReply( QString& msg );

  /**
     Format message to not contain any special formatting characters. 
     Used in plain text log.
  */
  bool formatLog( QString& msg );

  /** 
      Set the force default font flag. 
      This will not allow any font face changes, but instead will
      use kde defaults
  */
  void setForceDefaultFont(bool flag ) { isFontDefaultForced = flag; }

 private:

  /**
   * Scan for emoticons and replace it with <img source=...> tag.
   *
   * @param text QString reference of message to scan.
   */
  void parseIcons( QString& msg );
  
  /**
     Backs up the lexical analyizer 1 symbol. 
  */
  void backupLex( void );

  /**
     Checks each character in the lexical analyizer to see if it
     qualifies as a word.
  */
  lexTypes checkChar( int pos );

  /**
     Advances the lexical analyizer to the next symbol.
     @returns A symbol of the text parsed.
  */
  lexTypes lex( void );

  /**
     Converts the symbol to readable text. Used in debug messages.
     @param The symbol to convert to text.
  */
  const char* lex2string( lexTypes t );

  /**
     Main loop of the incoming message parser. 
  */
  bool ParseMsg( void );

  /**
     Main loop of log file parser.
  */
  bool ParseLog( void );

  /**
     Parses http url's.
  */
  bool httpParse( void );

  /**
     Helper function for urlParse. Parses out the url, the part
     after "http://"
  */
  bool urlParse( void );

  /**
     Parses <font tags. This method does not parse color attributes.
  */
  bool fontParse( void );

  /**
     Helper function for fontParse. This parses the font name.
     @param fname The font face name.
  */
  bool fontNameParse( QString& fname );

  /**
     Helper function for fontParse. This parses the font size.
     @param fname The font size string.
  */
  bool sizeParse( QString& fname );

  /**
     Parses font color tags.
  */
  bool escParse( void );

  /**
     Strip out font tags
  */
  bool fontStrip( void );
  
  /**
     String out ESC control tags
  */
  bool escStrip(void );

  /**
     Reset the lexical analyizer pointers.
   */
  void reset();

  /**
     Unit test for parser. Use this to verify output from parser is correct.
  */
  void testParse(void);

  /*************************/

  // these two must match up, color to RGB value
  enum YahooColor {  BLACK = 0, BLUE, LIGHTBLUE, GRAY, GREEN, 
		     PINK, PURPLE, ORANGE, RED, OLIVE };
  QStrList YahooTxtColor; // make match in constructor
    
  const QString fontString;
  const QString httpString;
  const QString sizeString;
  const QString faceString;

  // maintains offset into parsed string
  int lexPos;
  int pLexPos;
  lexTypes pLexType;
  QString pLexBuf;

  // global to maintain the lex state
  QString lexBuf;
  QString lexBufBkp;
  lexTypes sym;

  // keeps a nesting count of the font tags
  int fontCount;

  // count bold, italic, underline tags, for proper
  // tag termination when yahoo doesn't
  int _boldCount;
  int _italicCount;
  int _underlineCount;

  // ignore custom fonts incoming msgs?
  bool isFontDefaultForced;

  QValueStack<int> fontSizeStack;
  QValueStack<QString> colorNameStack;
  QValueStack<QString> fontNameStack;

  // message strings 
  QString inString;
  QString outString;

};


#endif
