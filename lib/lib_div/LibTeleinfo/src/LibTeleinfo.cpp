// **********************************************************************************
// Driver definition for French Teleinfo
// **********************************************************************************
// Creative Commons Attrib Share-Alike License
// You are free to use/extend this library but please abide with the CC-BY-SA license:
// http://creativecommons.org/licenses/by-sa/4.0/
//
// For any explanation about teleinfo ou use , see my blog
// http://hallard.me/category/tinfo
//
// Code based on following datasheet
// http://www.erdf.fr/sites/default/files/ERDF-NOI-CPT_02E.pdf
//
// Written by Charles-Henri Hallard (http://hallard.me)
//
// History : V1.00 2015-06-14 - First release
//           V2.00 2020-06-11 - Integration into Tasmota
//           V2.01 2020-08-11 - Merged LibTeleinfo official and Tasmota version
//                              Added support for new standard mode of linky smart meter
//           V2.02 2021-04-20 - Add label field to overload callback (ADPS)
//
// All text above must be included in any redistribution.
//
// Edit : Tab size set to 2 but I converted tab to sapces
//
// **********************************************************************************

#include "Arduino.h"
#include "LibTeleinfo.h" 

/* ======================================================================
Class   : TInfo
Purpose : Constructor
Input   : -
Output  : -
Comments: -
====================================================================== */
TInfo::TInfo()
{
  // Init of our linked list
  _valueslist.name = NULL;
  _valueslist.value = NULL;
  _valueslist.checksum = '\0';
  _valueslist.flags = TINFO_FLAGS_NONE;

  _separator = ' ';

  // callback
  _fn_ADPS = NULL;
  _fn_data = NULL;   
  _fn_new_frame = NULL;   
  _fn_updated_frame = NULL;   

  clearStats();
}

/* ======================================================================
Function: init
Purpose : try to guess 
Input   : Mode, historique ou standard
Output  : -
Comments: - 
====================================================================== */
void TInfo::init(_Mode_e mode)
{
  // free up linked list (in case on recall init())
  listDelete();

  // clear our receive buffer
  clearBuffer();

  // We're in INIT in term of receive data
  _state = TINFO_INIT;
 
  _mode = mode;
  if ( _mode == TINFO_MODE_STANDARD ) {
    _separator = TINFO_HT;
  } else {
    _separator = ' ';
  } 
}

/* ======================================================================
Function: clearStats
Purpose : clear stats counters
Input   : -
Output  : -
Comments: - 
====================================================================== */
void TInfo::clearStats()
{
   // reset Frame counters stats
  _checksumerror =0;
  _framesizeerror=0;
  _frameformaterror=0;
  _frameinterrupted=0;
}

/* ======================================================================
Function: attachADPS 
Purpose : attach a callback when we detected a ADPS on any phase
Input   : callback function
Output  : - 
Comments: -
====================================================================== */
void TInfo::attachADPS(void (*fn_ADPS)(uint8_t phase, char * label))
{
  // indicate the user callback
  _fn_ADPS = fn_ADPS;   
}

/* ======================================================================
Function: attachNewData 
Purpose : attach a callback when we detected a new/changed value 
Input   : callback function
Output  : - 
Comments: -
====================================================================== */
void TInfo::attachData(void (*fn_data)(ValueList * valueslist, uint8_t  state))
{
  // indicate the user callback
  _fn_data = fn_data;   
}

/* ======================================================================
Function: attachNewFrame 
Purpose : attach a callback when we received a full frame
Input   : callback function
Output  : - 
Comments: -
====================================================================== */
void TInfo::attachNewFrame(void (*fn_new_frame)(ValueList * valueslist))
{
  // indicate the user callback
  _fn_new_frame = fn_new_frame;   
}

/* ======================================================================
Function: attachChangedFrame 
Purpose : attach a callback when we received a full frame where data
          has changed since the last frame (cool to update data)
Input   : callback function
Output  : - 
Comments: -
====================================================================== */
void TInfo::attachUpdatedFrame(void (*fn_updated_frame)(ValueList * valueslist))
{
  // indicate the user callback
  _fn_updated_frame = fn_updated_frame;   
}

/* ======================================================================
Function: clearBuffer
Purpose : clear and init the buffer
Input   : -
Output  : -
Comments: - 
====================================================================== */
void TInfo::clearBuffer()
{
  // Clear our buffer, set index to 0
  memset(_recv_buff, 0, TINFO_BUFSIZE);
  _recv_idx = 0;
}


/* ======================================================================
Function: addCustomValue
Purpose : let user add custom values (mainly for testing)
Input   : Pointer to the label name
          pointer to the value
          pointer on flag state of the label 
Output  : pointer to the new node (or founded one)
Comments: checksum is calculated before adding, no need to bother with
====================================================================== */
ValueList * TInfo::addCustomValue(char * name, char * value, uint8_t * flags)
{
  // Little check
  if (name && *name && value && *value) {
    ValueList * me;

    // Same as if we really received this line
    customLabel(name, value, flags);
    me = valueAdd(name, value, calcChecksum(name,value), flags);

    if ( me ) {
      // something to do with new datas
      if (*flags & (TINFO_FLAGS_UPDATED | TINFO_FLAGS_ADDED | TINFO_FLAGS_ALERT) ) {
        // this frame will for sure be updated
        _frame_updated = true;
      }
      return (me);
    }
  }

  // Error or Already Exists
  return ( (ValueList *) NULL);
}

/* ======================================================================
Function: valueAdd
Purpose : Add element to the Linked List of values
Input   : Pointer to the label name
          pointer to the value
          checksum value
          flag state of the label (modified by function)
          string date (teleinfo format)
Output  : pointer to the new node (or founded one)
Comments: - state of the label changed by the function
====================================================================== */
ValueList * TInfo::valueAdd(char * name, char * value, uint8_t checksum, uint8_t * flags, char *horodate)
{
  // Get our linked list 
  ValueList * me = &_valueslist;

  uint8_t lgname = strlen(name);
  uint8_t lgvalue = strlen(value);
  uint8_t thischeck = calcChecksum(name,value,horodate);
  
  // just some paranoia 
  if (thischeck != checksum ) {
    TI_Debug(name);
    TI_Debug('=');
    TI_Debug(value);

    if (horodate && *horodate) {
      TI_Debug(F(" Date="));
      TI_Debug(horodate);
      TI_Debug(F(" "));
    }

    TI_Debug(F(" '"));
    TI_Debug((char) checksum);
    TI_Debug(F("' Not added bad checksum calculated '"));
    TI_Debug((char) thischeck);
    TI_Debugln(F("'"));
    AddLog(1, PSTR("LibTeleinfo::valueAdd Err checksum 0x%02X != 0x%02X"), thischeck, checksum);

  } else  {
    // Got one and all seems good ?
    if (me && lgname && lgvalue && checksum) {
      // Create pointer on the new node
      ValueList *newNode = NULL;
      ValueList *parNode = NULL ;
      uint32_t ts = 0;

      // Time stamped field?
      if (horodate && *horodate) {
        ts = horodate2Timestamp(horodate);
        // We don't check horodate (not used) on storage so re calculate checksum without this one
        checksum = calcChecksum(name,value) ;
      }

      // Loop thru the node
      while (me->next) {
        // save parent node
        parNode = me ;

        // go to next node
        me = me->next;

        // Check if we already have this LABEL (same name AND same size)
        if (lgname==strlen(me->name) && strcmp(me->name, name)==0) {
          if (ts) {
            me->ts = ts;
          }
          // Already got also this value  return US
          if (lgvalue==strlen(me->value) && strcmp(me->value, value) == 0) {
            *flags |= TINFO_FLAGS_EXIST;
            me->flags = *flags;
            return ( me );
          } else {
            // We changed the value
            *flags |= TINFO_FLAGS_UPDATED;
            me->flags = *flags ;
            // Do we have enought space to hold new value ?
            if (strlen(me->value) >= lgvalue ) {
              // Copy it
              strlcpy(me->value, value , lgvalue + 1 );
              me->checksum = checksum ;
              // That's all
              return (me);
            } else {
              // indicate our parent node that the next node
              // is not us anymore but the next we have
              parNode->next = me->next;

              // free up this node
              free (me);

              // Return to parent (that will now point on next node and not us)
              // and continue loop just in case we have sevral with same name
              me = parNode;
            }
          }
        }
      }

      // Our linked list structure sizeof(ValueList)
      // + Name  + '\0'
      // + Value + '\0'
      size_t size = sizeof(ValueList) + lgname + 1 + lgvalue + 1  ;

      // Create new node with size to store strings
      if ((newNode = (ValueList  *) malloc(size) ) == NULL)  {
        return ( (ValueList *) NULL );
      }

      // get our buffer Safe
      memset(newNode, 0, size);
      
      // Put the new node on the list
      me->next = newNode;

      // First String located after last struct element
      // Second String located after the First + \0
      newNode->checksum = checksum;
      newNode->name = (char *)  newNode + sizeof(ValueList);
      newNode->value = (char *) newNode->name + lgname + 1;

      // Copy the string data
      memcpy(newNode->name , name  , lgname );
      memcpy(newNode->value, value , lgvalue );
      // Add timestamp 
      newNode->ts = ts;

      // So we just created this node but was it new
      // or was matter of text size ?
      if ( (*flags & TINFO_FLAGS_UPDATED) == 0) {
        // so we added this node !
        *flags |= TINFO_FLAGS_ADDED ;
        newNode->flags = *flags;
      }

      TI_Debug(F("Added '"));
      TI_Debug(name);
      TI_Debug('=');
      TI_Debug(value);
      TI_Debug(F("' '"));
      TI_Debug((char) checksum);
      TI_Debugln(F("'"));

      // return pointer on the new node
      return (newNode);
    }

  } // Checksum OK


  // Error or Already Exists
  return ( (ValueList *) NULL);
}

/* ======================================================================
Function: valueRemoveFlagged
Purpose : remove element to the Linked List of values where 
Input   : paramter flags
Output  : true if found and removed
Comments: -
====================================================================== */
boolean TInfo::valueRemoveFlagged(uint8_t flags)
{
  boolean deleted = false;

  // Get our linked list 
  ValueList * me = &_valueslist;
  ValueList *parNode = NULL ;

  // Got one and all seems good ?
  if (me) {
    // Loop thru the node
    while (me->next) {
      // save parent node
      parNode = me ;

      // go to next node
      me = me->next;

      // found the flags?
      if (me->flags & flags ) {
        // indicate our parent node that the next node
        // is not us anymore but the next we have
        parNode->next = me->next;

        // free up this node
        free (me);

        // Return to parent (that will now point on next node and not us)
        // and continue loop just in case we have sevral with same name
        me = parNode;
        deleted = true;
      }
    }
  }

  return (deleted);
}

/* ======================================================================
Function: valueRemove
Purpose : remove element to the Linked List of values
Input   : Pointer to the label name
Output  : true if found and removed
Comments: -
====================================================================== */
boolean TInfo::valueRemove(char * name)
{
  boolean deleted = false;

  // Get our linked list 
  ValueList * me = &_valueslist;
  ValueList *parNode = NULL ;

  uint8_t lgname = strlen(name);

  // Got one and all seems good ?
  if (me && lgname) {
    // Loop thru the node
    while (me->next) {
      // save parent node
      parNode = me ;

      // go to next node
      me = me->next;

      // found ?
      if (strncmp(me->name, name, lgname) == 0) {
        // indicate our parent node that the next node
        // is not us anymore but the next we have
        parNode->next = me->next;

        // free up this node
        free (me);

        // Return to parent (that will now point on next node and not us)
        // and continue loop just in case we have sevral with same name
        me = parNode;
        deleted = true;
      }
    }
  }

  return (deleted);
}

/* ======================================================================
Function: valueGet
Purpose : get value of one element
Input   : Pointer to the label name
          pointer to the value where we fill data 
Output  : pointer to the value where we filled data NULL is not found
====================================================================== */
char * TInfo::valueGet(char * name, char * value)
{
  // Get our linked list 
  ValueList * me = &_valueslist;
  uint8_t lgname = strlen(name);

  // Got one and all seems good ?
  if (me && lgname) {

    // Loop thru the node
    while (me->next) {

      // go to next node
      me = me->next;

      // Check if we match this LABEL
      if (lgname==strlen(me->name) && strcmp(me->name, name)==0) {
        // this one has a value ?
        if (me->value) {
          // Check back checksum
          if (me->checksum == calcChecksum(me->name, me->value)) {
            // copy to dest buffer
            uint8_t lgvalue = strlen(me->value);
            strlcpy(value, me->value , lgvalue + 1 );
            return ( value );
          }
        }
      }
    }
  }
  // not found
  return ( NULL);
}

/* ======================================================================
Function: valueGet_P
Purpose : get value of one element
Input   : Pointer to the label name
          pointer to the value where we fill data 
Output  : pointer to the value where we filled data NULL is not found
====================================================================== */
char * TInfo::valueGet_P(const char * name, char * value)
{
  // Get our linked list 
  ValueList * me = &_valueslist;
  uint8_t lgname = strlen_P(name);

  // Got one and all seems good ?
  if (me && lgname) {

    // Loop thru the node
    while (me->next) {

      // go to next node
      me = me->next;

      // Check if we match this LABEL
      if (lgname==strlen(me->name) && strcmp_P(me->name, name)==0) {
        // this one has a value ?
        if (me->value) {
          // Check back checksum
          if (me->checksum == calcChecksum(me->name, me->value)) {
            // copy to dest buffer
            uint8_t lgvalue = strlen(me->value);
            strlcpy(value, me->value , lgvalue + 1 );
            return ( value );
          }
        }
      }
    }
  }
  // not found
  return ( NULL);
}

/* ======================================================================
Function: getTopList
Purpose : return a pointer on the top of the linked list
Input   : -
Output  : Pointer 
====================================================================== */
ValueList * TInfo::getList(void)
{
  // Get our linked list 
  return &_valueslist;
}

/* ======================================================================
Function: valuesDump
Purpose : dump linked list content
Input   : -
Output  : total number of values
====================================================================== */
uint8_t TInfo::valuesDump(void)
{
  // Get our linked list 
  ValueList * me = &_valueslist;
  uint8_t index = 0;
  uint8_t checksum=0;

  // Got one ?
  if (me) {
    // Loop thru the node
    while (me->next) {
      // go to next node
      me = me->next;

      index++;
      TI_Debug(index) ;
      TI_Debug(F(") ")) ;

      if (me->name ) {
        TI_Debug(me->name) ;
      } else {
        TI_Debug(F("NULL")) ;
      }

      TI_Debug(F("=")) ;

      if (me->value) {
        TI_Debug(me->value) ;
      } else {
        TI_Debug(F("NULL")) ;
      }

      if (me->name && me->value && *me->name && *me->value) {
        checksum = calcChecksum(me->name, me->value);
      }

      TI_Debug(F(" '")) ;
      TI_Debug(me->checksum) ;
      if (me->checksum != checksum ) {
        TI_Debug(F("'!Err ")); 
      } else {
        TI_Debug(F("' ")); 
      }

      // Flags management
      if ( me->flags) {
        TI_Debug(F("Flags:0x")); 
        TI_Debugf("%02X =>", me->flags); 
        if ( me->flags & TINFO_FLAGS_EXIST) {
          TI_Debug(F("Exist ")) ;
        }
        if ( me->flags & TINFO_FLAGS_UPDATED) {
          TI_Debug(F("Updated ")) ;
        }
        if ( me->flags & TINFO_FLAGS_ADDED) {
          TI_Debug(F("New ")) ;
        }
      }

      TI_Debugln() ;
    }
  }

  return index;
}

/* ======================================================================
Function: labelCount
Purpose : Count the number of label in the list
Input   : -
Output  : element numbers
====================================================================== */
int TInfo::labelCount()
{
  int count = 0;

  // Get our linked list 
  ValueList * me = &_valueslist;

  if (me)
    while ((me = me->next))
      count++;

  return (count);
}

/* ======================================================================
Function: listDelete
Purpose : Delete the ENTIRE Linked List, not a value
Input   : -
Output  : True if Ok False Otherwise
====================================================================== */
boolean TInfo::listDelete()
{
  // Get our linked list 
  ValueList * me = &_valueslist;

  // Got a pointer
  if (me) {
    ValueList *current;

    // For each linked list
    while ((current = me->next)) {
      // Get the next
      me->next =  current->next;

      // Free the current
      free(current);
    }

    // Free the top element
    me->next = NULL ;

    // Ok
    return (true);
  }

  return (false);
}

/* ======================================================================
Function: checksum
Purpose : calculate the checksum based on data/value fields
Input   : label name 
          label value 
          label timestamp
Output  : checksum
Comments: return '\0' in case of error

Group format in legacy mode (Mode Historique) - Last space not included in checksum
LF etiquette SP donnee SP Chk CR
0A           20        20     0D
   \____check________/


Group format in standard mode with timestamp (horodatage) - Last HT included in checksum
LF etiquette HT horodatage HT donnee HT Chk CR
0A           09            09        09     0D
   \____________checkum_______________/

Group format in standard mode without timestamp (horodatage) - Last HT included in checksum
LF etiquette HT donnee HT Chk CR
0A           09        09     0D
   \_____checkum________/

====================================================================== */
unsigned char TInfo::calcChecksum(char *etiquette, char *valeur, char * horodate) 
{
  char c;
  uint8_t sum = (_mode == TINFO_MODE_HISTORIQUE) ? _separator : (2 * _separator);  // Somme des codes ASCII du message + 2 separateurs

  // avoid dead loop, always check all is fine 
  if (etiquette && valeur) {
    // this will not hurt and may save our life ;-)
    if (strlen(etiquette) && strlen(valeur)) {
      while (*etiquette) {
        c =*etiquette++;
        // Add another validity check since checksum may not be sufficient
        if ( (c>='A' && c<='Z') || (c>='0' && c<='9') || c=='-' || c=='+') {
          sum += c ;
        } else {
          return 0;
        }
      }
  
      while(*valeur) {
        c = *valeur++ ;
        // Add another validity check since checksum may not be sufficient (space authorized in Standard mode)
        if ( (c>='A' && c<='Z') || (c>='0' && c<='9') || c==' ' || c=='.' || c=='-' || c=='+' || c=='/') {
          sum += c ;
        } else {
          return 0;
        }
      }

      if (horodate) {
        sum += _separator;
        c = *horodate++;
        // Add another validity check starting season [E]té (Summer) or [H]iver (Winter)
        if ( c=='E' || c=='H' || c=='e' || c=='h') {
          sum += c ;
          while (*horodate) {
            c = *horodate++ ;
            // Add another validity check for horodate digits
            if ( c>='0' && c<='9') {
              sum += c ;
            } else {
              return 0;
            }
          }
        } else {
          return 0;
        }
      }

      return ( (sum & 0x3f) + ' ' ) ;
    }
  }
  return 0;
}

/* ======================================================================
Function: horodate2Timestamp
Purpose : convert string date from frame to timestamp
Input   : pdate : pointer to string containing the date SAAMMJJhhmmss
                 season, year, month, day, hour, minute, second
Output  : unix format timestamp
Comments: 
====================================================================== */
uint32_t TInfo::horodate2Timestamp( char * pdate) 
{
  struct tm tm;
  time_t ts;
  char * p ;

  if (pdate==NULL || *pdate=='\0' || strlen(pdate)!=13) {
    return 0;
  }

  p = pdate + strlen(pdate) -2;
  tm.tm_sec  = atoi(p); *p='\0'; p-=2;
  tm.tm_min  = atoi(p); *p='\0'; p-=2;
  tm.tm_hour = atoi(p); *p='\0'; p-=2;
  tm.tm_mday = atoi(p); *p='\0'; p-=2;
  tm.tm_mon  = atoi(p); *p='\0'; p-=2;
  tm.tm_year = atoi(p) + 2000;

  tm.tm_year -= 1900;
  tm.tm_mon -= 1;
  tm.tm_isdst = 0;
  ts = mktime(&tm);
  if (ts == (time_t)-1) {
    TI_Debug(F("Failed to convert time "));
    TI_Debugln(pdate);
    return 0;
  }

  return (uint32_t) ts;
}

/* ======================================================================
Function: customLabel
Purpose : do action when received a correct label / value + checksum line
Input   : plabel : pointer to string containing the label
          pvalue : pointer to string containing the associated value
          pflags pointer in flags value if we need to cchange it
Output  : 
Comments: 
====================================================================== */
void TInfo::customLabel( char * plabel, char * pvalue, uint8_t * pflags) 
{
  int8_t phase = -1;

  // Monophasé
  if (strcmp(plabel, "ADPS")==0 ) 
    phase=0;

  // For testing
  //if (strcmp(plabel, "IINST")==0 ) {
  //  *pflags |= TINFO_FLAGS_ALERT;
  //}

  // triphasé c'est ADIR + Num Phase
  if (plabel[0]=='A' && plabel[1]=='D' && plabel[2]=='I' && plabel[3]=='R' && plabel[4]>='1' && plabel[4]<='3') {
    phase = plabel[4]-'0';
  }

  // Nous avons un ADPS ?
  if (phase>=0 && phase <=3) {
    // ne doit pas être sauvé définitivement
    *pflags |= TINFO_FLAGS_ALERT;
  
    // Traitement de l'ADPS demandé par le sketch
    if (_fn_ADPS) 
      _fn_ADPS(phase, plabel);
  }
}

/* ======================================================================
Function: checkLine
Purpose : check one line of teleinfo received
Input   : -
Output  : pointer to the data object in the linked list if OK else NULL
Comments: 
====================================================================== */
ValueList * TInfo::checkLine(char * pline) 
{
  char * p;
  char * ptok;
  char * pend;
  char * pvalue;
  char * pts;
  char   checksum;
  char  buff[TINFO_BUFSIZE];
  uint8_t flags  = TINFO_FLAGS_NONE;
  //boolean err = true ;  // Assume  error
  int len ; // Group len
  int i;
  int sep =0;
  bool hasts = false ;  // Assume timestamp on line

  if (pline==NULL) {
    //AddLog(3, PSTR("LibTeleinfo: Error pline==NULL"));
    return NULL;
  }

  len = strlen(pline); 

  // a line should be at least 7 Char
  // 2 Label + Space + 1 etiquette + space + checksum + \r
  if ( len < 7 || len >= TINFO_BUFSIZE) {
    //AddLog(3, PSTR("LibTeleinfo: Error len < 7 || len >= TINFO_BUFSIZE"));
    _framesizeerror++;
    return NULL;
  }

  p = &buff[0];
  sep = 0;
  // Get our own working copy and in the 
  // meantime, calculate separator count for
  // standard mode (to know if timestamped data)
  for (i=0 ; i<len ; i++) {
    // count separator, take care, checksum last one can be space separator
    if (*pline==_separator && *(pline+1)!='\r') {
      // Label + sep + Date + sep + Etiquette + sep + Checksum 
      if (++sep >=3){
        hasts = true;
      }
    }
    // Copy
    *p++ = *pline++;  
  }
  *p = '\0';

  p = &buff[0];
  ptok = p;       // for sure we start with token name
  pend = p + len; // max size

  // Init values
  pvalue = NULL;
  pts = NULL;
  checksum = 0;

  //TI_Debug("Got [");
  //TI_Debug(len);
  //TI_Debug("] ");
  
  // Loop in buffer 
  while ( p < pend ) {
    // start of token value
    if ( *p==_separator && ptok) {           
      // Isolate token name
      *p++ = '\0';

      // We have a timestamp
      // Label + sep + Date + sep + Etiquette + sep + Checksum 
      if (hasts) {
        if (!pts) {
          pts = p;
        } else {
          // 2nd separator, it's the label value
          if (!pvalue) {
            pvalue = p;
          } else {
            // 3rd separator so it's the checksum
            checksum = *p;
          }
        }

      // No timestamp
      // Label + sep + Etiquette + sep + Checksum 
      } else {
        // 1st separator, it's the label value
        if (!pvalue) {
          pvalue = p;
        } else {
          // 2nd separator so it's the checksum
          checksum = *p;
        }
      }
    }           

    // new line ? ok we got all we need ?
    if ( *p=='\r' ) {           
      *p='\0';

      // Good format ?
      if ( ptok && pvalue && checksum ) {
        // Always check to avoid bad behavior 
        if(strlen(ptok) && strlen(pvalue)) {
          // Is checksum is OK
          char calc_checksum = calcChecksum(ptok,pvalue,pts);
          
          if ( calc_checksum == checksum) {
            // In case we need to do things on specific labels
            customLabel(ptok, pvalue, &flags);

            // Add value to linked lists of values
            //AddLog(3, PSTR("LibTeleinfo: %s = %s"), ptok, pvalue);
            ValueList * me = valueAdd(ptok, pvalue, checksum, &flags, pts);

            // value correctly added/changed
            if ( me ) {
              // something to do with new datas
              if (flags & (TINFO_FLAGS_UPDATED | TINFO_FLAGS_ADDED | TINFO_FLAGS_ALERT) ) {
                // this frame will for sure be updated
                _frame_updated = true;

              }

              // Tasmota need to to calulation on Energy Module
              // So always pass data value even if it's the same than previous value
              // Do we need to advertise user callback
              if (_fn_data)
                _fn_data(me, flags);

            }
          }
          else
          {
            _checksumerror++;
            AddLog(1, PSTR("LibTeleinfo::checkLine Err checksum 0x%02X != 0x%02X (total errors=%d)"), calc_checksum, checksum, _checksumerror);
          }
        }
      } 
      else 
      {
        // Specific field not formated has others, don't set as an error
        if ( strcmp(ptok, "DATE") ) {
          _frameformaterror++;
          AddLog(1, PSTR("LibTeleinfo::checkLine frame format error total=%d"), _frameformaterror);
        } 
      }
    }           
    // Next char
    p++;

  } // While

  return NULL;
}

/* ======================================================================
Function: process
Purpose : teleinfo serial char received processing, should be called
          my main loop, this will take care of managing all the other
Input   : pointer to the serial used 
Output  : teleinfo global state
====================================================================== */
_State_e TInfo::process(char c)
{
   // be sure 7 bits only
   c &= 0x7F;

  // What we received ?
  switch (c)  {
    // start of transmission ???
    case  TINFO_STX:
      //AddLog(3, PSTR("LibTeleinfo: case TINFO_STX <<<<<<<<<<<<<<<<<<"));
      // Clear buffer, begin to store in it
      clearBuffer();

      // by default frame is not "updated"
      // if data change we'll set this flag
      _frame_updated = false;

      // We were waiting fo this one ?
      if (_state == TINFO_INIT || _state == TINFO_WAIT_STX ) {
          TI_Debugln(F("TINFO_WAIT_ETX"));
          //AddLog(3, PSTR("LibTeleinfo: state => TINFO_WAIT_ETX"));
         _state = TINFO_WAIT_ETX;
      } 
    break;

    // frame interruption
    case TINFO_EOT:
      //AddLog(3, PSTR("LibTeleinfo: case TINFO_EOT >>>>>>>>>>>>>>>>>>"));
      // discard incomplete frame
      // Clear buffer, begin to store in it
      clearBuffer();
      _frameinterrupted++;
      _state = TINFO_WAIT_STX;
    break;
 
    // End of transmission ?
    case  TINFO_ETX:
      //AddLog(3, PSTR("LibTeleinfo: case TINFO_ETX >>>>>>>>>>>>>>>>>>"));

      // Normal working mode ?
      if (_state == TINFO_READY) {
        // Get on top of our linked list 
        ValueList * me = &_valueslist;
        
        // Call user callback if any
        if (_frame_updated && _fn_updated_frame)
          _fn_updated_frame(me);
        else if (_fn_new_frame)
          _fn_new_frame(me);

        #ifdef TI_Debug
          valuesDump();
        #endif

        // It's important there since all user job is done
        // to remove the alert flags from table (ADPS for example)
        // it will be put back again next time if any
        valueRemoveFlagged(TINFO_FLAGS_ALERT);
      }

      // We were waiting fo this one ?
      if (_state == TINFO_WAIT_ETX) {
        TI_Debugln(F("TINFO_READY"));
        //AddLog(3, PSTR("LibTeleinfo: state => TINFO_READY"));
        _state = TINFO_READY;
      }
      else if ( _state == TINFO_INIT) {
        TI_Debugln(F("TINFO_WAIT_STX"));
        //AddLog(3, PSTR("LibTeleinfo: state => TINFO_WAIT_STX"));
        _state = TINFO_WAIT_STX ;
      }

    break;

    // Start of group \n ?
    case  TINFO_SGR:
      // Do nothing we'll work at end of group
      // we can safely ignore this char
      //AddLog(3, PSTR("LibTeleinfo: case TINFO_SGR _recv_idx=%d"), _recv_idx);
    break;

    // End of group \r ?
    case  TINFO_EGR:
      //AddLog(3, PSTR("LibTeleinfo: case TINFO_EGR _recv_idx=%d"), _recv_idx);

      // Are we ready to process ?
      if (_state == TINFO_READY) {
        // Store data recceived (we'll need it)
        if ( _recv_idx < TINFO_BUFSIZE) {
          _recv_buff[_recv_idx++]=c;
        } else {
          // group is too big (some ETX missing)
          _framesizeerror++;  
        }

        // clear the end of buffer (paranoia inside)
        memset(&_recv_buff[_recv_idx], 0, TINFO_BUFSIZE-_recv_idx);

        // check the group we've just received
        //AddLog(3, PSTR("LibTeleinfo: Group received %d bytes %s"), _recv_idx, _recv_buff);
        checkLine(_recv_buff) ;

        // Whatever error or not, we done
        clearBuffer();
      }
    break;

    // other char ?
    default:
    {
      // Only in a ready state of course
      if (_state == TINFO_READY) {
        // If buffer is not full, Store data 
        if ( _recv_idx < TINFO_BUFSIZE)
          _recv_buff[_recv_idx++]=c;
        else {
          AddLog(1, PSTR("LibTeleinfo: _recv_idx = %d/%d buffer overflow"), _recv_idx, TINFO_BUFSIZE);
          clearBuffer();
       }
      }
    }
    break;
  }

  return _state;
}


