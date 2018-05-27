#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

typedef struct ts_cmd_cntr	ts_cmd_cntr;
typedef struct ts_cmd_hdl	ts_cmd_hdl;
typedef union  un_cmd_data	un_prv_data;


struct ts_cmd_hdl {

	un_prv_data 	*pPrvt;	
	
	ts_cmd_cntr 	*pCmdContainer;

	unsigned char	(*pIncommingHdl)	(ts_cmd_hdl * self);
	unsigned char	(*pOutgoingHdl)		(ts_cmd_hdl * self);
	void		 	(*pErrorHdl)		(ts_cmd_hdl * self, unsigned char );

	unsigned char	(*available)		(ts_cmd_hdl * self);
	void 			(*run)				(ts_cmd_hdl * self);
	void			(*push)				(ts_cmd_hdl * self, const unsigned char *x, unsigned int len);
	void*			(*getParam)			(ts_cmd_hdl * self);
	unsigned int 	(*Return) 			(ts_cmd_hdl * self, unsigned char oknok, unsigned char* p);
};

struct ts_cmd_cntr {
	const unsigned char cmd[3];
	unsigned int 	(*exe)				(ts_cmd_hdl * self);
};

typedef struct cm_init_ts {
	unsigned char 	(*inc)(ts_cmd_hdl * self);
	unsigned char 	(*out)(ts_cmd_hdl * self)	;
	void		 	(*err)(ts_cmd_hdl * self, unsigned char c)	;

	ts_cmd_cntr		*container;	

	unsigned char	fbyte;	
	unsigned char 	len;
	unsigned char	sizeBuf;
}cm_init_ts;

ts_cmd_hdl 	cmNew_Command(cm_init_ts * init );

//#include "cpp/CommandHandler.cpp"
#endif //COMMAND_HANDLLER_H