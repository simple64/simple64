/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus - cheat.c                                                 *
 *   Mupen64Plus homepage: http://code.google.com/p/mupen64plus/           *
 *   Copyright (C) 2009-2010 Richard Goedeken                              *
 *   Copyright (C) 2010 Rhett Osborne (spinout)                            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cheat.h"
#include "common.h"
#include "core_commands.h"

/* local definitions */
#define CHEAT_FILE	"mupencheat.txt"

/* local variables */
static char              *l_IniText = NULL;
char              *l_CheatGameName = NULL;
sCheatInfo        *l_CheatList = NULL;
int                l_CheatCodesFound = 0;
int                l_RomFound = 0;

/*********************************************************************************************************
 *  Static (Local) functions
 */

static int isSpace(char ch)
{
    return (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n');
}

/* Find cheat code */
sCheatInfo *CheatFindCode(int Number)
{
    sCheatInfo *pCur = l_CheatList;
    while (pCur != NULL)
    {
        if (pCur->Number == Number) break;
        pCur = pCur->Next;
    }
    return pCur;
}


/* Activate a code */
void CheatActivate(sCheatInfo *pCheat)
{
    int i;

    /* Get a m64p_cheat_code object */
    m64p_cheat_code * code = (m64p_cheat_code*) calloc(pCheat->Count, sizeof(m64p_cheat_code));
    if (code == NULL)
    {
        DebugMessage(M64MSG_WARNING, "could not allocate memory for code '%s'", pCheat->Name);
        return;
    }
    /* Fill in members */
    for (i = 0; i < pCheat->Count; i++)
    {
        code[i].address = pCheat->Codes[i].address;
        code[i].value = pCheat->Codes[i].variables[pCheat->Codes[i].var_to_use];
    }
    /* Enable cheat */
    if (CoreAddCheat(pCheat->Name, code, pCheat->Count) != M64ERR_SUCCESS)
    {
        DebugMessage(M64MSG_WARNING, "CoreAddCheat() failed for cheat code %i (%s)", pCheat->Number, pCheat->Name);
        free(code);
        return;
    }

    free(code);
    DebugMessage(M64MSG_STATUS, "activated cheat code %i: %s", pCheat->Number, pCheat->Name);
}

void CheatFreeAll(void)
{
    if (l_IniText != NULL)
        free(l_IniText);
    l_IniText = NULL;

    sCheatInfo *pCur = l_CheatList;
    while (pCur != NULL)
    {
        sCheatInfo *pNext = pCur->Next;
        if (pCur->Codes != NULL)
        {
            int i;
            for (i=0; i < pCur->Count; i++)
            {
                if (pCur->Codes[i].variables != NULL)
                    free(pCur->Codes[i].variables);
                if (pCur->Codes[i].variable_names != NULL)
                    free(pCur->Codes[i].variable_names);
            }
            free(pCur->Codes);
        }
        free(pCur);
        pCur = pNext;
    }

    l_CheatList = NULL;
    l_RomFound = 0;
    l_CheatCodesFound = 0;
}

/* Append new code */
static sCheatInfo * NewCode(char *CheatName, int CheatNum)
{
    /* allocate memory for a new sCheatInfo struct */
    sCheatInfo *pNew = (sCheatInfo *) malloc(sizeof(sCheatInfo));
    if (pNew == NULL) return NULL;

    /* fill in the data members */
    pNew->Number = CheatNum;
    pNew->Count = 0;
    pNew->VariableLine = -1;
    pNew->Name = CheatName;
    pNew->Description = NULL;
    pNew->Codes = NULL;
    pNew->Next = NULL;
    pNew->active = false;

    l_CheatCodesFound++;

    /* stick it at the end of the list */
    if (l_CheatList == NULL)
    {
        l_CheatList = pNew;
        return pNew;
    }
    sCheatInfo *pLast = l_CheatList;
    while (pLast->Next != NULL) pLast = pLast->Next;
    pLast->Next = pNew;
    return pNew;
}

static void CheatAddVariables(cheat_code * Code, char *varlist)
{
    /* needs to be more verbose? */
    Code->variables = NULL;
    Code->variable_names = NULL;
    Code->var_count = 0;
    while (*varlist != 0)
    {
        if ((Code->variables = (int*) realloc(Code->variables, sizeof(int) * (Code->var_count + 1))) == NULL)
            return;
        if ((Code->variable_names = (char**) realloc(Code->variable_names, sizeof(char*) * (Code->var_count + 1))) == NULL)
            return;
        if (sscanf(varlist, "%04X", &Code->variables[Code->var_count]) != 1)
            Code->variables[Code->var_count] = 0;
        if (strchr(varlist, '"') == NULL)
            return;
        Code->variable_names[Code->var_count] = strchr(varlist, '"') + 1;
        if ((varlist = strchr(Code->variable_names[Code->var_count], '"')) == NULL)
            return;
        *varlist++ = 0;
        if (*varlist == ',')
            varlist++;
        Code->var_count++;
    }
}

/*********************************************************************************************************
* global functions
*/

void ReadCheats(char *RomSection)
{
    sCheatInfo *curr_code = NULL;
    const char *romdbpath = ConfigGetSharedDataFilepath(CHEAT_FILE);
    if (romdbpath == NULL)
    {
        DebugMessage(M64MSG_WARNING, "cheat code database file '%s' not found.", CHEAT_FILE);
        return;
    }

    /* read the INI file into a new buffer */
    FILE *fPtr = NULL;
    fPtr = fopen(romdbpath, "rb");
    if (fPtr == NULL)
    {   
        DebugMessage(M64MSG_WARNING, "Couldn't open cheat code database file '%s'.", romdbpath);
        return;
    }
    fseek(fPtr, 0L, SEEK_END);
    size_t IniLength = (size_t)ftell(fPtr);
    fseek(fPtr, 0L, SEEK_SET);
    l_IniText = (char *) malloc(IniLength + 1);
    if (l_IniText == NULL)
    {
        DebugMessage(M64MSG_WARNING, "Couldn't allocate %li bytes of memory to read cheat file.", IniLength);
        fclose(fPtr);
        return;
    }
    if (fread(l_IniText, 1, IniLength, fPtr) != IniLength)
    {
        DebugMessage(M64MSG_WARNING, "Couldn't read %li bytes from cheat file.", IniLength);
        free(l_IniText);
        l_IniText = NULL;
        fclose(fPtr);
        return;
    }
    fclose(fPtr);
    l_IniText[IniLength] = 0; /* null-terminate the text data */

    /* parse lines from cheat database */
    char *curline = NULL;
    char *nextline = l_IniText;

    while(nextline != NULL && *nextline != 0)
    {
        curline = nextline;
        /* get pointer to next line and NULL-terminate the current line */
        nextline = strchr(curline, '\n');
        if (nextline != NULL)
        {
            *nextline = 0;
            nextline++;
        }

        /* remove leading and trailing white space */
        while(isSpace(*curline)) curline++;
        char *endptr = curline + strlen(curline) - 1;
        while(isSpace(*endptr)) *endptr-- = 0;

        /* ignore line if comment or empty */
        if (*curline == '#' || strncmp(curline, "//", 2) == 0 || *curline == 0)
            continue;

        /* handle beginning of new rom section */
        if (strncmp(curline, "crc ", 4) == 0)
        {
            /* if we have already found cheats for the given ROM file, then exit upon encountering a new ROM section */
            if (l_RomFound && (l_CheatGameName != NULL || l_CheatList != NULL))
                return;
            /* else see if this Rom Section matches */
            if (strcmp(curline+4, RomSection) == 0)
                l_RomFound = 1;
            continue;
        }

        /* if we haven't found the specified ROM section, then continue looking */
        if (!l_RomFound)
            continue;

        /* Game name */
        if (strncmp(curline, "gn ", 3) == 0)
        {
            l_CheatGameName = curline+3;
            continue;
        }

        /* code name */
        if (strncmp(curline, "cn ", 3) == 0)
        {
            curr_code = NewCode(curline + 3, l_CheatCodesFound);
            if (curr_code == NULL)
                DebugMessage(M64MSG_WARNING, "error getting new code (%s)", curline+3);
            continue;
        }
        
        /* if curr_code is NULL, don't do these checks */
        if (curr_code == NULL)
            continue;

        /* code description */
        if (strncmp(curline, "cd ", 3) == 0)
        {
            curr_code->Description = curline+3;
            continue;
        }

        /* code line */
        int address;
        if (sscanf(curline, "%8X %*s", &address) == 1)
        {
            curr_code->Codes = (cheat_code*) realloc(curr_code->Codes, sizeof(cheat_code) * (curr_code->Count + 1));
            if (strncmp(curline+9, "????", 4) == 0)
            {
                curr_code->Codes[curr_code->Count].var_count = 0;
                CheatAddVariables(&curr_code->Codes[curr_code->Count], curline+14);
                curr_code->VariableLine = curr_code->Count;
            }
            else
            {
                int var;
                curr_code->Codes[curr_code->Count].var_count = 1;
                curr_code->Codes[curr_code->Count].variables = (int*) malloc(sizeof(int));
                if(curr_code->Codes[curr_code->Count].variables == NULL)
                {
                    DebugMessage(M64MSG_WARNING, "couldn't allocate memory; ignoring line: '%s'", curline);
                    continue;
                }
                if (sscanf(curline+9, "%04X", &var) != 1)
                    var = 0;
                curr_code->Codes[curr_code->Count].variables[0] = var;
                curr_code->Codes[curr_code->Count].variable_names = NULL;
            }
            curr_code->Codes[curr_code->Count].var_to_use = 0;
            curr_code->Codes[curr_code->Count].address = address;
            curr_code->Count++;
            continue;
        }

        /* otherwise we don't know what this line is */
        DebugMessage(M64MSG_WARNING, "unrecognized line in cheat file: '%s'", curline);
    }

}
