#pragma once

#ifndef UPC_Export
#ifdef UPC_DLL_EXPORTS
#define UPC_Export __declspec(dllexport)
#else
#define UPC_Export __declspec(dllimport)
#endif
#endif


/**
 * return code
 */
enum _IniRetCode_e { 
	INI_SUCCESS 			= 0 , 
	INI_FILE_OPEN_ERR 		= -1, 
	INI_NOT_FOUND_SECTION 	= -2, 
	INI_NOT_FOUND_KEY 		= -3, 
	INI_NOT_FOUND 			= -4, 
	INI_NO_SUCH_TYPE 		= -5, 
	INI_INVALID_ARG 		= -6, 
	INI_EMPTY_STRING 		= -7, 
	INI_NOT_SECTION_NAME 	= -8, 
	INI_INVALID_SYNTAX 		= -9, 
	INI_FSEEK_ERROR 		= -10, 
	INI_FTELL_ERROR 		= -11, 
	INI_FPUT_ERROR 			= -12, 
	INI_FILE_NOT_UPDATE 	= -13, 
	INI_NO_SECTION_LIST 	= -14, 
	INI_NO_KEY_LIST 		= -15, 
	INI_REACHED_END 		= -16, 
	INI_UNKNOWN 			= -99
};



/**
 * ini type
 *
 * value type
 */
enum IniType_e { 
	INI_STRING, 			/**< string type of value */
	INI_DECIMAL,			/**< decimal type of value */ 
	INI_HEXA 				/**< hexa type of value */
};


/**
 * creating mode
 */
enum IniCreateMode_e {
	INI_CREATE_SECTION_KEY,   	/**< create section and key  */
	INI_CREATE_KEY,				/**< only create or update key  */
	INI_CREATE_NONE				/**< only update */
};



/**
 * ini tuple
 * 
 * tupe = key + value
 */
typedef struct _IniTuple_s {
	char key[100] ;				/**< the array of key */
	char value[100] ;			/**< the array of value */
} IniTuple_s ;


/**
 * maximum line size
 */
#define INI_MAX_LINE_SIZE 2048

/**
 * maximum section number
 */
#define INI_MAX_SEC_NUM 250

/**
 * maximum key number
 */
#define INI_MAX_KEY_NUM 250

class UPC_Export CubeBaseini
{
public:
	CubeBaseini(void);
	~CubeBaseini(void);

	static CubeBaseini* GetInstance();

	int		ini_get_value(char *ini_fname, char *sec_name, char *key_name, void *buf, int buf_len, IniType_e type );
	int		ini_put_value(char *ini_fname, char *section, char *key, void *value, IniType_e type, IniCreateMode_e mode);
	int		ini_get_section_list(char *ini_fname, char **buf, int buf_len);
	int		ini_get_key_list(char *ini_fname, char *sec_name, char **buf, int buf_len);
	int		ini_get_section(char *ini_fname, char *sec_name, IniTuple_s *buf);
	int		ini_get_key_first(char *ini_fname, char *sec_name, char *buf, int buf_len);
	int		ini_get_key_next(char *ini_fname, int offset, char *buf, int buf_len);
	int		ini_get_section_first(char *ini_fname, char *buf, int buf_len);
	int		ini_get_section_next(char *ini_fname, int offset, char *buf, int buf_len);
	int		ini_remove_section(char *ini_fname, char *sec_name);
	int		ini_remove_key(char *ini_fname, char *sec_name, char *key_name);
};

