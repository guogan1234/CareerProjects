/*
 @Copyright Reserved by XXXX.
 Path definition and operations. Path is a string with colons and dots as delimiters.
 Colons are nodes separators, and dots are separators of node and attribute, and separator
 of attribute and attribute. Examples:
    "tableName"
    "tableName.fieldName"
    "domain_label"
    "domain_label.field"
    "domain_label.tableName.field"
    ...
    "domain_label:station_label:pro_system_label:device_label:ai_label"
    "domain_label:station_label:pro_system_label:device_label:ai_label.field"
    "domain_label:station_label:pro_system_label:device_label:ai_label.tableName.field"

 Create by KanWenDi, 2018.04.02.
 Histories:

 */
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "rtdb_cli_path.h"

struct rtdb_cli_path
{
    int8_t nodeNum;
    char **nodes;
    int8_t fieldNum;
    char **fields;
};

rtdb_cli_path* rtdb_cli_path_create()
{
    rtdb_cli_path *path = malloc(sizeof(*path));
    if (path)
    {
        memset(path, 0, sizeof(*path));
    }
    return path;
}

static void rtdb_cli_path_free_strings(char **strings, int8_t size)
{
    if (strings)
    {
        int8_t i;
        for(i = 0; i < size; i++)
        {
            if (strings[i])
            {
                free(strings[i]);
            }
        }
        free(strings);
    }
}

void rtdb_cli_path_free(rtdb_cli_path *path)
{
    if (path)
    {
        if(path->nodes)
            rtdb_cli_path_free_strings(path->nodes, path->nodeNum);
        if(path->fields)
            rtdb_cli_path_free_strings(path->fields, path->fieldNum);
        free(path);
    }
}

int8_t rtdb_cli_path_get_nodesize(const rtdb_cli_path *path)
{
    return (path ? path->nodeNum : 0);
}

int8_t rtdb_cli_path_get_fieldsize(const rtdb_cli_path *path)
{
    return (path ? path->fieldNum : 0);
}

static const char* crtdb_cli_path_get_key(char **keys, int8_t keyNum, int8_t index)
{
    if (!keys || keyNum == 0 || index < 0 || index >= keyNum)
    {
        return NULL;
    }
    return keys[index];
}

const char* crtdb_cli_path_get_node(const rtdb_cli_path *path, int8_t index)
{
    return crtdb_cli_path_get_key(path ? path->nodes : NULL, path->nodeNum, index);
}

const char* rtdb_cli_path_get_field(const rtdb_cli_path *path, int8_t index)
{
    return crtdb_cli_path_get_key(path ? path->fields : NULL, path->fieldNum, index);
}

static int rtdb_cli_path_add_key(char ***keys, int8_t *keyNum, const char *key)
{
    char **tmp = realloc(*keys, sizeof(char*)*(*keyNum+1));
    if (!tmp)
    {
        return 2;
    }

    tmp[*keyNum] = strdup(key);
    *keys = tmp;
    *keyNum += 1;

    return 0;
}

int rtdb_cli_path_add_node(rtdb_cli_path *path, const char *node)
{
    if (path && node)
    {
        return rtdb_cli_path_add_key(&path->nodes, &path->nodeNum, node);
    }
    return 1;
}

int rtdb_cli_path_add_field(rtdb_cli_path *path, const char *field)
{
    if (path && field)
    {
        return rtdb_cli_path_add_key(&path->fields, &path->fieldNum, field);
    }
    return 1;
}

static int rtdb_cli_path_parse_base(
    rtdb_cli_path *path,const char *spath,char ch)
{
    char *newPath = NULL;
    int i, start, end, psLen;
    int ec = -1;;
    int nodeFlag = (ch == ':');
    char pBuf[256];
    
    psLen = spath ? strlen(spath) : 0;
    if (!psLen)  return ec;
        
    newPath = malloc(psLen+2);
    if(!newPath) return ec;
    memset(newPath,0,psLen+2);
    memcpy(newPath,spath,psLen);
    newPath[psLen] = ch;
    
    psLen += 1;    
    start = end = 0;
    for(i = 0;i < psLen;i++)
    {
        int diff = 0;
        char cur = newPath[i];
        if(ch == cur)
        {
            end = i;
            diff = end - start;
            //rtdb_debug_trace("end : %d , start : %d , diff : %d\n",end,start,diff);
            memset(pBuf,0,256);
            memcpy(pBuf,newPath + start,diff);

            if (nodeFlag)
            {
                ec = rtdb_cli_path_add_node(path, pBuf);
            }
            else
            {
                ec = rtdb_cli_path_add_field(path, pBuf);
            }   
            //rtdb_debug_trace("pBuf : %s\n",pBuf);
            if (ec != 0) 
            {
                break; 
            }
            start = end+1;
        }
    }    
        
    free(newPath);
    return ec;
}


static int rtdb_cli_path_parse_add(rtdb_cli_path *path,const char *spath,int iPos,char ch)
{
    int ec = -1;
    char *pBuf;
    pBuf = malloc(iPos + 1);
    if(!pBuf) return ec;
    memset(pBuf,0,iPos + 1);
    memcpy(pBuf,spath,iPos);
    ec = rtdb_cli_path_parse_base(path,pBuf,ch);
    free(pBuf); 
    return ec;
}

rtdb_cli_path* rtdb_cli_path_parse(const char *spath)
{
    const char ch = ':',ch2 = '.';
    int i=0,iPos=0,end=0,psLen=0;
    rtdb_cli_path *path = NULL;
    int ec;

    psLen = spath ? strlen(spath) : 0;
    if (!psLen)  return NULL;
    
    path = rtdb_cli_path_create();
    if (!path) return NULL; 
    
    for(i = 0; i < psLen;i++)
    {
        iPos = i;
        if(ch2 == spath[i]) break;
    }
    
    //rtdb_debug_trace("iPos : %d , psLen : %d\n",iPos,psLen);
    if(iPos+1 == psLen)
    {
        // node only        
        ec = rtdb_cli_path_parse_base(path,spath,ch);
        //rtdb_debug_trace("path : %s\n",spath);
       // rtdb_debug_trace("ch : %c\n",ch);
    }
    else
    {
        // node and attribute        
        int iDiff = 0;
        ec = rtdb_cli_path_parse_add(path,spath,iPos,ch);
        if(ec != 0)
        {
            rtdb_cli_path_free(path);
            return NULL;
        }
        
        iDiff = psLen - iPos;
        ec = rtdb_cli_path_parse_add(path,spath+iPos,iDiff,ch2);
        if(ec != 0)
        {
            rtdb_cli_path_free(path);
            return NULL;
        }
    }
    return path;
}

