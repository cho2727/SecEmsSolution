#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "common/CubeBaseini.h"

int ini_search_section (FILE *fp, char *section);
int ini_search_key (FILE *fp, char *key);
int ini_search_key2(FILE *fp, char *key);
int ini_seperate_section_name (char *line, char * buff);
int ini_separate_key_tuple(char *tuple, char *left_str, char *right_str);
int ini_convert_integer(int *buf, char *num_str);
long ini_create_key(FILE *fp, long cur_pos, char *key, char *value, IniType_e type);
int ini_create_section_put_value(FILE *fp, char *section, char *key, char *value, IniType_e type);
void ini_remove_white_space(char *line);

#ifdef USE_PEX
void ini_replace_env_pattern(char *str);
#endif

/**
 * search section [XXX]
 * 
 * @param[in]   fp  file pointer
 * @param[in]   section  section name
 * @return   return code
 */
int ini_search_section (FILE *fp, char *section)
{
#ifdef USE_PEX
	char 	line[2048];									/* read line from file */
#else
	char 	line[200];									/* read line from file */
#endif
	char 	tmp[100];									/* store section name */
	int		len = INI_NOT_FOUND_SECTION;

	for (;;) {
		if ( fgets(line, sizeof(line), fp) == NULL ) /* End Of File */
			return INI_NOT_FOUND_SECTION; 				 /* Can't find section */

		len = strlen(line);
		ini_remove_white_space(line);

		if(line[0] != '[') 						/* is section beginning mark ? */	
			continue;	   						/* Skipp */

		if ( ini_seperate_section_name(line, tmp) < 0) /* get section name */
			continue; 

#ifdef USE_PEX
		ini_replace_env_pattern(tmp);
#endif

		if ( strcmp(tmp, section) == 0) 		/* find Section */
			return len; 
	}

	return len;
}


/**
 * search key
 * 
 * @param[in]   fp  file pointer
 * @param[in]   key  key name
 * @return   return code
 */
int ini_search_key (FILE *fp, char *key)
{
	char  line[2048];
	int   line_len;
	char  key_str[2048];
	char  value_str[2048];
	long  prev_offset;

	prev_offset = ftell(fp);		// return current offset
	if (prev_offset < 0)
		return INI_FTELL_ERROR;

	while (1) 
	{
		if (fgets(line, sizeof(line), fp) == NULL) 	/* End Of File */
			return INI_NOT_FOUND_KEY;					/* Can't find key*/ 

		line_len = strlen(line);

		ini_remove_white_space(line);

		if (line[0] == '[')
		{
#if 0
			offset = ftell(fp);
			offset = offset - strlen(line) - 1;
			if (fseek(fp, offset, SEEK_SET) < 0)
				return INI_FSEEK_ERROR;
#else
			if (fseek(fp, prev_offset, SEEK_SET) < 0)
				return INI_FSEEK_ERROR;
#endif
			return INI_NOT_FOUND_KEY;		/* Can't find key*/
		}

		/* separate key and value */
		if (ini_separate_key_tuple(line, key_str, value_str) == INI_SUCCESS)
		{
			if (strcmp(key, key_str) == 0) 
			{
				if (fseek(fp, prev_offset, SEEK_SET) < 0)
					return INI_FSEEK_ERROR;
				return line_len; 						/* return line length */
			}
		}	

		prev_offset += line_len;
	}

	return INI_NOT_FOUND_KEY;
}


/**
 * search key2
 * 
 * @param[in]   fp  file pointer
 * @param[in]   key  key name
 * @return   return code
 */
int ini_search_key2(FILE *fp, char *key)
{
	char line [200];
	char left_str[100];									/* decompose key */
	char right_str[100];								/* decompose value */
	int len;

	for (;;) {
		if ( fgets(line, sizeof(line), fp) == NULL) 	/* End Of File */
			return INI_NOT_FOUND_KEY;					/* Can't find key*/ 

		len = strlen(line);

		ini_remove_white_space(line);


		if(line[0] == '['){
			int offset;
			offset = ftell(fp);
			offset = offset - strlen(line) - 1;
			fseek(fp, offset, SEEK_SET);
			return INI_NOT_FOUND_KEY;		/* Can't find key*/
		}

		/* separate key and value */
		if(ini_separate_key_tuple(line, left_str, right_str) == INI_SUCCESS)
		{
			if (strcmp(key, left_str) != 0) continue;
			return len; 						/* return line length */
		}	
	}

	return INI_NOT_FOUND_KEY;
}


/**
 * extract section name in line
 * 
 * @param[in]   line  line including section name
 * @param[out]  buff  the array for section name buffered
 * @return   return code
 */
int ini_seperate_section_name (char *line, char * buff)
{
	int mark; 	/* indicate Position of ']' */
	int cursor; /* indexing variable */ 

	if (line == NULL) return  INI_INVALID_ARG;			/* Invalid Argument */
	if (strlen(line) == 0) return INI_EMPTY_STRING;	/* empty string */
	if (line[0] != '[') return INI_NOT_SECTION_NAME;	/* line  do not contain section name */

	for (cursor = strlen(line); cursor > 0; cursor--) 
		if (line[cursor] == ']') break;

	if (cursor == 0) return INI_NOT_SECTION_NAME;		/* Invalid section name */

	mark = cursor;

	for (cursor = 0; cursor+1 < mark; cursor++)  /* copy section name */
		buff[cursor] = line[cursor+1];

	buff[cursor] = 0;

	ini_remove_white_space(buff);

	return INI_SUCCESS;
}


/**
 * extract key and value
 * 
 * @param[in]   tuple  tuple including key and value
 * @param[out]  left_str   the array for key name buffered
 * @param[out]  right_str  the array for value buffered
 * @return   return code
 */
int ini_separate_key_tuple(char *tuple, char *left_str, char *right_str)
{
    int index = 0; 		/* temp index */
	int	ls_index = 0; 	/* left_string index */
	int	rs_index = 0;	/* right_str index */
    int q_flag = 0;  	/* "...."  flag */
	int f_flag = 0;		/* first flag */


    if(strstr(tuple, "=") == NULL) return INI_INVALID_SYNTAX;

    /* get key name */
    for(left_str[0] = 0; tuple[index] != '\0' ; index++)
    {
        if(tuple[index] == '=') break;

		/* skip white space char  */
        if(tuple[index] == ' ' || tuple[index] == '\t')
			continue;

        left_str[ls_index++] = tuple[index];
        left_str[ls_index] = 0;
    }

    index++; /* skip... '=' */

    /* get value, q_flag = '"', f_flag = first_flag */
    for(right_str[0] = 0;;)
    {
        if(q_flag == 0 && tuple[index] == ';') break; /* end */

        if(q_flag == 1 && (tuple[index] == 0 || tuple[index] == '\n')) 
			return INI_INVALID_SYNTAX; /* invalid syntax */

		/* empty value */
        if(f_flag == 1 && (tuple[index] == 0 || tuple[index] == '\n')) break;

		/* first white space block ... skip */
        if(q_flag == 0 && (tuple[index] == ' ' || tuple[index] == '\t'))
        {
            index++;
            continue;
        }

		/* begin '"' */
        if(f_flag == 0 && tuple[index] == '"') 
        {
            f_flag = 1, q_flag = 1;
        }
		/* end '"' */
#if	0
        else if(q_flag == 1 && tuple[index] == '"')
        {
            break;
        }
        else 
        {
            f_flag = 1;
            right_str[rs_index++] = tuple[index];
            right_str[rs_index] = 0;
        }
#else
        else if(q_flag == 1 && tuple[index] == '"')
        {
            q_flag = 0;
        }
        {
            f_flag = 1;
            right_str[rs_index++] = tuple[index];
            right_str[rs_index] = 0;
        }
#endif

        index++;
    }
    return INI_SUCCESS ;
}

/**
 * convert string to integer
 * 
 * @param[out]   buf  the array for intefer buffered
 * @param[in]   num_str  number in string type
 * @return   return code
 */
int ini_convert_integer(int *buf, char *num_str )
{
	int			i;

	/*---- 16 scale (0-9, A-F, a-f) ----*/
	if(memcmp(num_str, "0x", 2) == 0)
	{
		for(i = 2;num_str[i] != 0; i++)
		{
			/*---- INVALID CHAR ----*/
			if(!((num_str[i] >= '0' && num_str[i] <= '9') ||
				(num_str[i] >= 'A' && num_str[i] <= 'F') ||
				(num_str[i] >= 'a' && num_str[i] <= 'f')))
			{
				return INI_INVALID_SYNTAX;
			}
		}	
		sscanf(num_str, "%x", buf);
	}
	/*---- 10 scale (0-9) ----*/
	else
	{
		for(i = 0;num_str[i] != 0; i++)
        {   
            /*---- INVALID CHAR ----*/
			if(!(num_str[i] >= '0' && num_str[i] <= '9'))
			{
				return INI_INVALID_SYNTAX;
			}
		}
		*buf = atoi(num_str);
	}

	return INI_SUCCESS;
}

/**
 * create key
 * 
 * @param[in]   fp  file pointer
 * @param[in]   cur_pos  current position of file pointer
 * @param[in]   key  key name
 * @param[in]   value  value
 * @param[in]   type  value type
 * @return      return code
 */
long ini_create_key(FILE *fp, long cur_pos, char *key, char *value, IniType_e type)
{
	FILE   *tmp_fp;						/* temporary File Pointer */
	//long   cur_pos = ftell(fp);		/* store current file position */
	long   tmp_write_size; 
	long   new_write_size;
	char   line[INI_MAX_LINE_SIZE];			/* line buffer */
	char   new_line[INI_MAX_LINE_SIZE];
	char   tmp[512], *tmpfilename;
	int	   int_val;
	long   ret;
	
	/* new key-value */
	switch (type) 
	{
	case INI_STRING :
		sprintf(new_line, "%s=%s\n", key, value);
		break;
	case INI_DECIMAL :
		memcpy(&int_val, value, sizeof(int_val));
		sprintf(new_line, "%s=%d\n", key, int_val);
		break;
	case INI_HEXA :
		memcpy(&int_val, value, sizeof(int_val));
		sprintf(new_line, "%s=%x\n", key, int_val);
		break;
	}

	/* 
	 * copy new key-value and last file-data to tempfile 
	 */
	tmpfilename = tmpnam(tmp);
	tmp_fp = fopen(tmpfilename, "w+");	/* Create Temporary File */
	if (tmp_fp == NULL) {
		return INI_FILE_OPEN_ERR;
	}

	if (fputs(new_line, tmp_fp) == EOF) {
		ret = INI_FPUT_ERROR;
		goto labelERROR;
	}
	new_write_size = strlen(new_line);
	
	tmp_write_size = 0;
	while (fgets(line, sizeof(line), fp))
	{
		if (fputs(line, tmp_fp) == EOF) 
		{
			ret = INI_FPUT_ERROR;
			goto labelERROR;
		}	

		tmp_write_size += strlen(line);
	}
	//tmp_write_size = ftell(tmp_fp);

	/* 
	 * copy saved tempfile to origin file after new_created_line 
	 */
	if (fseek(tmp_fp, 0, SEEK_SET) < 0) {
		ret = INI_FSEEK_ERROR;
		goto labelERROR;
	}
	if (fseek(fp, cur_pos, SEEK_SET) < 0) {
		ret = INI_FSEEK_ERROR;
		goto labelERROR;
	}

	while (fgets(line, sizeof(line), tmp_fp))
	{
		if (fputs(line, fp) == EOF) 
		{
			ret = INI_FPUT_ERROR;
			goto labelERROR;
		}
	}

	ret = new_write_size + tmp_write_size;

labelERROR:
	fclose(tmp_fp);
#ifdef	WIN32
	_unlink(tmpfilename); /* delete Temporary File */
#else
	unlink(tmpfilename); /* delete Temporary File */
#endif
	return ret;
} 


/**
 * create section put value
 * 
 * @param[in]   fp  file pointer
 * @param[in]   section  section name
 * @param[in]   key  key name
 * @param[in]   value  value
 * @param[in]   type  value type
 * @return      return code
 */
int ini_create_section_put_value (FILE *fp, char *section, char *key, char *value,
								IniType_e type)
{
	char 	buff[INI_MAX_LINE_SIZE];								/* store line */
	int		int_val;


	sprintf(buff,"[%s]\n", section);
	if (fputs(buff, fp) == EOF)  {
		return INI_FILE_NOT_UPDATE; 	/* write section name */
	}
	
	if (type == INI_STRING)
		sprintf(buff,"%s=%s\n", key, value);
	else if (type == INI_DECIMAL)
	{
		memcpy(&int_val, value, sizeof(int_val));
		sprintf(buff,"%s=%d\n", key, int_val);
	}
	else if (type == INI_HEXA)
	{
		memcpy(&int_val, value, sizeof(int_val));
		sprintf(buff, "%s=%x", key, int_val); 
	}
	else 
	{
		return INI_FILE_NOT_UPDATE;
	}

	if (fputs(buff, fp) == EOF) 
	{
		return INI_FILE_NOT_UPDATE; 	/*write key value */
	}

	return INI_SUCCESS;
		
}


/**
 * remove white space
 * 
 * @param[in]   line  pointer of string 
 */
void ini_remove_white_space(char *line)
{
	int     i, j;
	int     len = strlen(line)+1;
	char	tmpline[1024] = {0};
	int		isQuota = 0;

	if (len == 0 ) return;


	/* remove left space */
	for(i=0; i<(int)strlen(line); i++)
	{
		if(isspace((int)line[i])) continue;
		else
		{
			memmove( line, &line[i], len - i );
			break;
		}
	}


	/* remove right space */
	for(i=strlen(line)-1; i>=0; i--)
	{
		if(isspace((int)line[i]) || line[i] == '\n')
		{
			line[i] = '\0';
		}
		else
		{
			break;
		}
	}

#if	0	// changed 
	// added 8 lines by nold0001@nate.com 2012-07-24(ȭ)
	for( i=0 ; i<(int)strlen(line) ; ++i )
	{
		if( line[i] == '#' || line[i] == ';' )
		{
			line[i] = 0;
			break;
		}
	}
#endif
    for( i=0,j=0 ; i<(int)strlen(line) ; ++i )
    {    
        if( line[i] == '"' )
        {    
            if( isQuota == 0 )  isQuota = 1; 
            else                isQuota = 0; 
    //      isQuota = isQuota==1?0:1;
            continue;
        }    

        if( isQuota == 0 )
        {    
            if( line[i] == '#' || line[i] == ';' )
            {    
            //  tmpline[j] = 0;
                break;
            }    
        }    

        tmpline[j++] = line[i];
    }    
    strcpy( line, tmpline );

	return;
}

#ifdef USE_PEX

/**
 * replace envirionment pattern
 * 
 * @param   str  pointer of string to be changed
 */
void ini_replace_env_pattern(char *str)
{
	char *p, *p2;

	if (str &&
		(p = strchr(str, '$')) && p[1] == '{' &&
		(p2 = strchr(&p[2], '}')) && p + 2 < p2)  {

		char *e, buf[2048];
		int n;

		/* get environment vairable value */
		*p2 = '\0';
		if ((e = getenv(&p[2])) == NULL)
			e = (char*)"";

		/* construct new string */
		if (p > str)
			memcpy(buf, str, p - str);	
		n = strlen(e);
		memcpy(&buf[p - str], e, n);
        strcpy(&buf[(p - str) + n], &p2[1]); 

		/* copy back to str */
		strcpy(str, buf);
	}
}
#endif


CubeBaseini::CubeBaseini(void)
{
}


CubeBaseini::~CubeBaseini(void)
{
}


CubeBaseini* CubeBaseini::GetInstance( void )  
{
	static CubeBaseini	_Instance;
	return &_Instance;
}



/**
 * get value.
 * 
 * @param[in]   ini_fname  ini file name
 * @param[in]   sec_name  section name
 * @param[in]   key_name  key name
 * @param[out]   buf  the array to put the value into
 * @param[in]   buf_len  length of buf
 * @param[in]   type  value type
 * @return      return code 
 */
int CubeBaseini::ini_get_value( char *ini_fname, char *sec_name, char *key_name,
		                                   void *buf, int buf_len, IniType_e type )
{

	FILE *	fp; 				/* FILE pointer for INI file */
	char 	line[10000];			/* temp buff for fgets */
	char	left_str[10000];		/* temp buff for key string */
	char	right_str[10000];		/* temp buff for value string */
	char 	key[100]; 			/* key name */ 
	int 	key_idx;			/* index for searching a key */
	int		ret;

	fp = fopen(ini_fname, "rb");
    if(fp == NULL) return INI_FILE_OPEN_ERR;

	if (ini_search_section(fp, sec_name) < 0) /* search the section */
	{									
		fclose(fp);
		return INI_NOT_FOUND_SECTION;			/* the section  does not exist */
	}


	for(;;)	/* find a key */
	{

		if(fgets(line, sizeof(line), fp) == NULL) 
       	{
			fclose(fp);
       		//return INI_SUCCESS;
			return INI_NOT_FOUND;
        }

		if(line[0] == '[')			/* reached the next section */
		{							/* it means there's no such key */
			fclose(fp);
			return INI_NOT_FOUND;
		}

		for(key_idx = 0 ; line[key_idx] != '\0' ; key_idx++) /* get a key name */
		{
			if (line[key_idx] == '=')
				break;
			key[key_idx] = line[key_idx];
		}
		key[key_idx] = 0;
	
		ini_remove_white_space(key);	/* remove white spaces */
		if(strcmp(key, key_name) != 0) 	/* compare to given key_name */
			continue;
	
		if(ini_separate_key_tuple(line, left_str, right_str) == INI_SUCCESS)
   		{
           	if(strcmp(key_name, left_str) == 0)	/* the key's found */
           	{
#ifdef USE_PEX
				ini_replace_env_pattern(right_str);
#endif
           		switch(type)
           	 	{
               		case INI_DECIMAL:  /* in case of Decimal Type */
						ini_remove_white_space(right_str);
						ret = ini_convert_integer((int *) buf, right_str);
						if(ret != INI_SUCCESS )
						{
							fclose(fp);	
							return ret;
						}
                   		break;

                    case INI_STRING:  /* in case of String Type */
						ini_remove_white_space(right_str);
						if(strlen(right_str) + 1 > (size_t)buf_len)
						{
							memcpy(buf, right_str, buf_len-1);
							((char *)buf)[buf_len-1] = '\0';
						}
						else
   	                     	strcpy((char*)buf, right_str);
   	                	break;
   	           		case INI_HEXA:	/* in case of Hexa Type */
					{
/*
						char 		*endptr;
						long		long_val;
						long_val = strtoul(key_str, &end_ptr, 16);
*/
					}
					break; 
					default: 
						fclose(fp);
						return INI_NO_SUCH_TYPE; 
				}
				fclose(fp);
            	return INI_SUCCESS;
           	}
       	}		
		fclose(fp);
		return INI_SUCCESS;
	}
}




/**
 * put value.
 * 
 * @param[in]   ini_fname  ini file name
 * @param[in]   section  section name
 * @param[in]   key  key name
 * @param[in]   value  pointer of value
 * @param[in]   type  value type
 * @param[in]   mode  creating mode
 * @return      return code 
 */
int CubeBaseini::ini_put_value(char *ini_fname, char *section, char *key, void *value, IniType_e type, IniCreateMode_e mode)
{
	FILE   *fp; /* FILE pointer for INI file */
	int    err;
	char   *v = (char*) value;

	/* verify parameters */
	if ( !(key && value && key[0]) 
			|| ( type == INI_STRING 
				&& !(v[0] && strlen(key)+strlen(v) < INI_MAX_LINE_SIZE) ) )
	{
		return INI_INVALID_ARG;
	}

	fp = fopen(ini_fname, "r+");
	if (fp == 0) 
	{
		fp = fopen(ini_fname, "w+");
		if (fp == 0)
		{
			return INI_FILE_OPEN_ERR;
		}
	}

	if (ini_search_section(fp, section) != INI_NOT_FOUND_SECTION) 
		/* section already exist */
	{
		long saved_file_size;
		long offset;			/* file offset */

		/* find key point */
		err = ini_search_key(fp, key);
		if (err < 0)  /* search key fail */
		{
			if (mode == INI_CREATE_NONE) {
				fclose(fp);
				return INI_NOT_FOUND_KEY; 
			}

			offset = ftell(fp);
			if (offset < 0)
				offset = 0;
		}
		else
		{
			char line[10000];

			offset = ftell(fp);
			if (offset < 0)
				offset = 0;

			/* skip current key line */
			if (fgets(line, sizeof(line), fp) == 0) { 
				err = INI_FSEEK_ERROR;
				goto labelERROR;
			}
		}

		saved_file_size = ini_create_key(fp, offset, key, (char*)value, type);
		if (saved_file_size < 0) {
			err = INI_FSEEK_ERROR;
			goto labelERROR;
		}

		fclose(fp);
#ifdef	WIN32
#else
		truncate((const char*)ini_fname, offset+saved_file_size);
#endif
		return INI_SUCCESS;
	}
	else if (mode == INI_CREATE_SECTION_KEY) 
		/* create section & key. then put value to the key */
	{ 
		err = ini_create_section_put_value(fp, section, key, (char*)value, type);	
		if (err < 0) {
			err = INI_FILE_NOT_UPDATE;
			goto labelERROR;
		}

		fclose(fp);
		return INI_SUCCESS;
	}
	else /* secition does not exist 
			while mode setting is CREATE_KEY or CREATE_NONE */
	{
		fclose(fp);
		return INI_NOT_FOUND_SECTION;
	} 


labelERROR:
	fclose(fp);

	return err;
}




/**
 * get section list.
 * 
 * @param[in]   ini_fname  ini file name
 * @param[out]   buf  buffer which section list is put to
 * @param[in]   buf_len  length of buf
 * @return      return code 
 */
int CubeBaseini::ini_get_section_list(char *ini_fname, char **buf, int buf_len)
{
	FILE *	fp; 						/* FILE pointer for INI file */
	char 	line[10000];				/* temp buff for fgets */
	char 	tmp_buf[INI_MAX_SEC_NUM][100];	/* temp buff for saving section list */
	int 	sec_idx1;
	int 	sec_idx2;
	int 	sec_list_idx;				/* the index for section list */

	memset(tmp_buf, 0, sizeof(tmp_buf));

	fp = fopen(ini_fname, "rb");
    if(fp == NULL) return INI_FILE_OPEN_ERR;

	sec_list_idx = 0;

	for(;;)	
	{	
		if(fgets(line, sizeof(line), fp) == NULL)	/* no more data */
		{
			fclose(fp);
			break;
		}

		ini_remove_white_space(line);			/* remove white spaces */

		if(line[0] != '[')	/* skip if it's not the section beginning mark */
			continue;
		else								/* is section beginning mark? */
		{
#ifdef USE_PEX
			ini_replace_env_pattern(line);
#endif
			for(sec_idx1 = 0,sec_idx2 = 1 ; ; sec_idx1++, sec_idx2++) 
			{										/* get a section name */
				if(line[sec_idx2] == ']') 			/* valid end mark */
				{
					break;
				}

				if(line[sec_idx2] == '\n' 			/* invalid end mark */
					|| line[sec_idx1] == 0)			/* invalid end mark */
				{
					return INI_INVALID_SYNTAX;
				}

				tmp_buf[sec_list_idx][sec_idx1] = line[sec_idx2]; 
			}
			strcpy((char *)((char *)buf + buf_len*sec_list_idx), tmp_buf[sec_list_idx]);
			sec_list_idx++;
		}

	}


	if (sec_list_idx > 0)				/* return the total section number */
		return sec_list_idx;		
	else return INI_NO_SECTION_LIST; 

}

/**
 * get key list.
 * 
 * @param[in]   ini_fname  ini file name
 * @param[in]   sec_name  section name
 * @param[out]   buf  buffer which key list is put to
 * @param[in]   buf_len  length of buf
 * @return      return code 
 */
int CubeBaseini::ini_get_key_list(char *ini_fname, char *sec_name, char **buf, int buf_len)
{
	FILE *	fp; 						/* FILE pointer for INI file */
	char 	line[10000];					/* temp buffer for fgets */
	char 	tmp_buf[INI_MAX_KEY_NUM][100]; 	/* temp bufer for saving key list */
	int 	key_idx;
	int 	key_list_idx;				/* the index for key list */


	memset(tmp_buf, 0, sizeof(tmp_buf));

	fp = fopen(ini_fname, "rb");
    if(fp == NULL) return INI_FILE_OPEN_ERR;

	key_list_idx = 0;

	if (ini_search_section(fp, sec_name) <= 0) /* search the section */
	{								
		fclose(fp);
		return INI_NOT_FOUND_SECTION;			/* the section  does not exist */
	}

	/* if the given section name does exist in te ini_file */

	for(;;)
	{
		if(fgets(line, sizeof(line), fp) == NULL)	/* no more data */
		{
			break; 
		}

		ini_remove_white_space(line);	 /* remove white spaces */

		if(line[0] == '[') 					/* reached the next section */
			break;		
		else if(line[0] == ';') 			/* skip comment */
			continue;		
		else
		{
			for(key_idx = 0 ; line[key_idx] != '\0' ; key_idx++) /* get a key name */
			{								
				if (line[key_idx] == '=')	/* valid key-value tuple */
				{
					break;	
				}
		
				if (line[key_idx] == '\n' 	/* if there's no '=' mark */
					|| line[key_idx] == 0)	/* invalid key-value tuple */ 
				{
					fclose(fp);
					return INI_INVALID_SYNTAX;
				}

				tmp_buf[key_list_idx][key_idx] = line[key_idx];
			}

			strcpy((char  *)((char *)buf + buf_len*key_list_idx), tmp_buf[key_list_idx]);
			key_list_idx++;  

		}
	}

	fclose(fp);

	if (key_list_idx > 0)				/* return the total key number */
		return key_list_idx;	
	else 
		return INI_NO_KEY_LIST;

}

/**
 * get section.
 * 
 * @param[in]   ini_fname  ini file name
 * @param[in]   sec_name  section name
 * @param[out]  buf the array of IniTuple to be put the value in
 * @return      return code 
 */
int CubeBaseini::ini_get_section(char *ini_fname, char *sec_name, IniTuple_s *buf)
{
	FILE *			fp; 						/* FILE pointer for INI file */
	char 			line[10000];					/* temp buff for fgets */
	char   			left_str[10000];      		/* temp buff for key string */
    char		    right_str[10000];     		/* temp buff for value string */
	IniTuple_s		tmp_buf[INI_MAX_KEY_NUM]; 		/* temp bufer for saving key list */
	int 			key_list_idx;				/* the index for key list */

	memset(tmp_buf, 0, sizeof(tmp_buf));

	fp = fopen(ini_fname, "rb");
    if(fp == NULL) return INI_FILE_OPEN_ERR;

	key_list_idx = 0;

	if (ini_search_section(fp, sec_name) < 0) /* search the section */
	{									
		fclose(fp);
		return INI_NOT_FOUND_SECTION;			/* the section  does not exist */
	}

	for(;;)	
	{	
		if(fgets(line, sizeof(line), fp) == NULL)	/* no more data */
		{
			break; 
		}

		ini_remove_white_space(line);	 /* remove white spaces */
		if(strlen(line) == 0) continue;

		if(line[0] == '[') 				/* reached the next section */
			break;		
		else 
		{
			if(strstr(line, "=") == NULL) {
				fclose(fp);
				return INI_INVALID_SYNTAX;
			}

			if(ini_separate_key_tuple(line, left_str, right_str) == INI_SUCCESS)
			{
#ifdef USE_PEX
				ini_replace_env_pattern(right_str);
#endif
				strcpy(buf[key_list_idx].key, left_str);
				strcpy(buf[key_list_idx].value, right_str);

				key_list_idx++;
			}
		}
	}

	fclose(fp);

	if (key_list_idx > 0)  		/* return the total key number */
		return key_list_idx;
	else 
		return INI_NO_KEY_LIST;
}

/**
 * get key first.
 * 
 * @param[in]   ini_fname  ini file name
 * @param[in]   sec_name  section name
 * @param[out]  buf  pointer of key string
 * @param[out]  buf_len  length of buf
 * @return      offset of fp(file pointer)
 */
int CubeBaseini::ini_get_key_first(char *ini_fname, char *sec_name, char *buf, int buf_len)
{
	FILE *		fp;
	char 		line[10000];
	int			key_idx;
	int			offset;

	memset(buf, 0, buf_len);

	fp = fopen(ini_fname, "rb");
    if(fp == NULL) return INI_FILE_OPEN_ERR;

	if (ini_search_section(fp, sec_name) <= 0) /* search the section */
    {
        fclose(fp);
        return INI_NOT_FOUND_SECTION;          /* the section  does not exist */
    }

	for(;;)
    {
        if(fgets(line, sizeof(line), fp) == NULL)   /* no more data */
        {
            fclose(fp);
            return	INI_REACHED_END;
        }

		ini_remove_white_space(line);     /* remove white spaces */

		if(strlen(line) == 0) continue;

		if(line[0] == '[')              /* reached the next section */
		{
            fclose(fp);
            return	INI_REACHED_END;
		}
        else
        {
			for(key_idx = 0 ; line[key_idx] != '\0' ; key_idx++) /* get a key name */
            {
                if (line[key_idx] == '=')   /* valid key-value tuple */
                {
                    break;
                }

                if (line[key_idx] == '\n'   /* if there's no '=' mark */
                    || line[key_idx] == 0)  /* invalid key-value tuple */
                {
            		fclose(fp);
                    return INI_INVALID_SYNTAX;
                }

				if(key_idx < buf_len-1) buf[key_idx] = line[key_idx];
            }
			break;
        }
	}
	offset = (int) ftell(fp);
	fclose(fp);

	ini_remove_white_space(buf);
	return offset;
}

/**
 * get key next.
 * 
 * @param[in]   ini_fname  ini file name
 * @param[in]   offset  offset of fp(file pointer)
 * @param[out]  buf  pointer of key string
 * @param[out]  buf_len  length of buf
 * @return      offset of fp(file pointer)
 */
int CubeBaseini::ini_get_key_next(char *ini_fname, int offset, char *buf, int buf_len)
{
	FILE *		fp;
	char 		line[10000];
	int			key_idx;

	memset(buf, 0, buf_len);

	fp = fopen(ini_fname, "rb");
    if(fp == NULL) return INI_FILE_OPEN_ERR;

	fseek(fp, offset, 0);
	for(;;)
    {
        if(fgets(line, sizeof(line), fp) == NULL)   /* no more data */
        {
            fclose(fp);
            return	INI_REACHED_END;
        }

		ini_remove_white_space(line);     /* remove white spaces */

		if(strlen(line) == 0) continue;

		if(line[0] == '[')              /* reached the next section */
		{
			fclose(fp);
            return	INI_REACHED_END;
		}
        else
        {
			for(key_idx = 0 ; line[key_idx] != '\0' ; key_idx++) /* get a key name */
            {
                if (line[key_idx] == '=')   /* valid key-value tuple */
                {
                    break;
                }

                if (line[key_idx] == '\n'   /* if there's no '=' mark */
                    || line[key_idx] == 0)  /* invalid key-value tuple */
                {
					fclose(fp);
                    return INI_INVALID_SYNTAX;
                }

				if(key_idx < buf_len-1) buf[key_idx] = line[key_idx];
            }
			break;
        }
	}
	offset = (int) ftell(fp);
	fclose(fp);

	ini_remove_white_space(buf);
	return offset;
}

/**
 * get section first
 * 
 * @param[in]   ini_fname  ini file name
 * @param[out]  buf  pointer of section string
 * @param[out]  buf_len  length of buf
 * @return      offset of fp(file pointer)
 */
int CubeBaseini::ini_get_section_first(char *ini_fname, char *buf, int buf_len)
{
	FILE *	fp; 						/* FILE pointer for INI file */
	char 	line[10000];				/* temp buff for fgets */
	int 	sec_idx1;
	int 	sec_idx2;
	int		offset;

	memset(buf, 0, buf_len);

	fp = fopen(ini_fname, "rb");
    if(fp == NULL) return INI_FILE_OPEN_ERR;

	for(;;)	
	{	
		if(fgets(line, sizeof(line), fp) == NULL)	/* no more data */
		{
			fclose(fp);
			return INI_REACHED_END;
		}

		ini_remove_white_space(line);			/* remove white spaces */

		if(line[0] != '[')	/* skip if it's not the section beginning mark */
			continue;
		else								/* is section beginning mark? */
		{
#ifdef USE_PEX
			ini_replace_env_pattern(line);
#endif
			for(sec_idx1 = 0,sec_idx2 = 1 ; ; sec_idx1++, sec_idx2++) 
			{										/* get a section name */
				if(line[sec_idx2] == ']') 			/* valid end mark */
				{
					break;
				}

				if(line[sec_idx2] == '\n' 			/* invalid end mark */
					|| line[sec_idx1] == 0)			/* invalid end mark */
				{
					fclose(fp);
					return INI_INVALID_SYNTAX;
				}

				if(sec_idx1 < buf_len -1) buf[sec_idx1] = line[sec_idx2]; 
			}
			break;
		}

	}
	offset = (int) ftell(fp);
	fclose(fp);

	ini_remove_white_space(buf);			/* remove white spaces */

	return offset;
}

/**
 * get section next
 * 
 * @param[in]   ini_fname  ini file name
 * @param[in]   offset of fp(file pointer)
 * @param[out]  buf  pointer of section string
 * @param[out]  buf_len  length of buf
 * @return      offset of fp(file pointer)
 */
int CubeBaseini::ini_get_section_next(char *ini_fname, int offset, char *buf, int buf_len)
{
	FILE *	fp; 							/* FILE pointer for INI file */
	char 	line[10000];					/* temp buff for fgets */
	int 	sec_idx1;
	int 	sec_idx2;

	memset(buf, 0, buf_len);

	fp = fopen(ini_fname, "rb");
    if(fp == NULL) return INI_FILE_OPEN_ERR;

	fseek(fp, offset, 0);

	for(;;)	
	{	
		if(fgets(line, sizeof(line), fp) == NULL)	/* no more data */
		{
			fclose(fp);
			return INI_REACHED_END;
		}

		ini_remove_white_space(line);			/* remove white spaces */

		if(line[0] != '[')	/* skip if it's not the section beginning mark */
			continue;
		else								/* is section beginning mark? */
		{
#ifdef USE_PEX
			ini_replace_env_pattern(line);
#endif
			for(sec_idx1 = 0,sec_idx2 = 1 ; ; sec_idx1++, sec_idx2++) 
			{										/* get a section name */
				if(line[sec_idx2] == ']') 			/* valid end mark */
				{
					break;
				}

				if(line[sec_idx2] == '\n' 			/* invalid end mark */
					|| line[sec_idx1] == 0)			/* invalid end mark */
				{
					fclose(fp);
					return INI_INVALID_SYNTAX;
				}

				if(sec_idx1 < buf_len -1) buf[sec_idx1] = line[sec_idx2]; 
			}
			break;
		}

	}
	offset = (int) ftell(fp);
	fclose(fp);

	ini_remove_white_space(buf);			/* remove white spaces */

	return offset;
}

/**
 * remove section
 * 
 * @param[in]   ini_fname  ini file name
 * @param[in]   sec_name  section name
 * @return      return code
 */
int CubeBaseini::ini_remove_section(char *ini_fname, char *sec_name)
{
	FILE			*rfp, *wfp ;
	int				length;
	long 			woffset, roffset;
	char			line[200];
	struct stat		file_stat;
	char			*w_buff;	


	rfp = fopen (ini_fname, "r");
	if (rfp == NULL) {
		return INI_FILE_OPEN_ERR;
	}

	stat(ini_fname, &file_stat);
	w_buff = (char *) malloc(file_stat.st_size);

	length = ini_search_section(rfp, sec_name);
	if (length == INI_NOT_FOUND_SECTION){
		fclose(rfp);
		free(w_buff);
		return INI_NOT_FOUND_SECTION;
	}
	roffset = ftell(rfp);
	woffset = roffset - length; 
	rewind(rfp);
	fread(w_buff, woffset, 1, rfp);
	fseek(rfp, roffset, SEEK_SET);

	/* find next section */
	for (;;) {
		if ( fgets(line, sizeof(line), rfp) == NULL ){
			fclose(rfp);

			wfp = fopen(ini_fname, "w+");
			if ( wfp == NULL) {
				free(w_buff);
				return INI_FILE_OPEN_ERR;
			}
			setvbuf(wfp, NULL, _IONBF, 0);
			fwrite(w_buff, woffset, 1, wfp);
			free(w_buff);
			fclose(wfp);

			return INI_SUCCESS;
		}
		length = strlen(line);	
		ini_remove_white_space(line);
		if(line[0] == '[') {
			roffset = ftell(rfp);
			roffset = roffset - length;
			break;
		}
	}

	fseek(rfp, roffset, SEEK_SET);
	fread((w_buff+woffset), file_stat.st_size - roffset, 1, rfp );
	woffset = woffset + file_stat.st_size - roffset ;
	fclose(rfp);

	wfp = fopen(ini_fname, "w+");
	if ( wfp == NULL) {
		free(w_buff);
		return INI_FILE_OPEN_ERR;
	}
	setvbuf(wfp, NULL, _IONBF, 0);
	fwrite(w_buff, woffset, 1, wfp);

	free(w_buff);
	fclose(wfp);
	return INI_SUCCESS;
}

/**
 * remove key
 * 
 * @param[in]   ini_fname  ini file name
 * @param[in]   sec_name  section name
 * @param[in]   key_name  section name
 * @return      return code
 */
int CubeBaseini::ini_remove_key(char *ini_fname, char *sec_name, char *key_name)
{
	FILE			*rfp, *wfp ;
	int				length;
	long 			woffset, roffset;
	struct stat		file_stat;
	char			*w_buff;	


	rfp = fopen (ini_fname, "r");

	if (rfp == NULL) {
		return INI_FILE_OPEN_ERR;
	}
	stat(ini_fname, &file_stat);
	w_buff = (char *) malloc(file_stat.st_size);

	length = ini_search_section(rfp, sec_name);
	if (length == INI_NOT_FOUND_SECTION){
		fclose(rfp);
		free(w_buff);
		return INI_NOT_FOUND_SECTION;
	}
	length = ini_search_key2(rfp, key_name);
	if (length == INI_NOT_FOUND_KEY){
		fclose(rfp);
		free(w_buff);
		return INI_NOT_FOUND_KEY;
	}
	roffset = ftell(rfp);
	woffset = roffset - length; 
	rewind(rfp);
	fread(w_buff, woffset, 1, rfp);
	fseek(rfp, roffset, SEEK_SET);
	fread((w_buff+woffset), file_stat.st_size - roffset, 1, rfp );
	woffset = woffset + file_stat.st_size - roffset ;
	fclose(rfp);

	wfp = fopen(ini_fname, "w+");
	if ( wfp == NULL) {
		return INI_FILE_OPEN_ERR;
	}
	setvbuf(wfp, NULL, _IONBF, 0);
	fwrite(w_buff, woffset, 1, wfp);

	free(w_buff);
	fclose(wfp);
	return INI_SUCCESS;
}