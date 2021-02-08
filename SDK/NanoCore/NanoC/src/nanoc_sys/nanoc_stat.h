/*
   --------------------------------------------------------------------------
   nanoc_stat.h               NanoC File Headers (PSP/PSPE)
   --------------------------------------------------------------------------
   This file is part of NanoC headers. If the user is compiling using Nanodesktop,
   the compiler will replace the file normally used by SDK with this automatically

   NanoCore Libraries Collection provides you ANSI-compliant libc and
   and libm for using with Nanodesktop routines. 
   
   See file Readme.rtf in NanoCore root dir for details.
   
   The following code has been obtained from different sources. We indicate
   the sources, the author and the original license. 
   -----------------------------------------------------------------------
*/   

#ifndef _NANOC_SYS_STAT_H_

                    #define  _NANOC_SYS_STAT_H_     

                    #ifdef __cplusplus
                    extern "C" {
                    #endif
                         
                            
                    #include <psptypes.h>           // For definire il tipo u8
               
                    #include <_ansi.h>
                    #include <time.h>
                    #include <sys/types.h>
                    
                    // Prototipo della struttura FILE: non può essere richiamato
                    // da stdio.h qui, perchè ciò crea problemi durante la fase
                    // di compilazione della NanoC
                    
                    /* ensure FILE is defined. */
                    #ifndef __FILE_DEFINED
                    #define __FILE_DEFINED
                    typedef struct 
                    {
                      int  type;
                      int  fd;
                      int  cnt;
                      int  flag;
                      int  len;
                      int  has_putback;
                      u8   putback;
                    } FILE;
                    #endif // __FILE_DEFINED
        
                    // Struttura stat
                    
                    struct	stat 
                    {
                      dev_t		st_dev;
                      ino_t		st_ino;
                      mode_t	st_mode;
                      nlink_t	st_nlink;
                      uid_t		st_uid;
                      gid_t		st_gid;
                      dev_t		st_rdev;
                      off_t		st_size;
                    
                      time_t	st_atime;
                      time_t	st_mtime;
                      time_t	st_ctime;
                    };
                    
                    
                    #define	    _IFMT	0170000	/* type of file */
                    #define		_IFDIR	0040000	/* directory */
                    #define		_IFCHR	0020000	/* character special */
                    #define		_IFBLK	0060000	/* block special */
                    #define		_IFREG	0100000	/* regular */
                    #define		_IFLNK	0120000	/* symbolic link */
                    #define		_IFSOCK	0140000	/* socket */
                    #define		_IFIFO	0010000	/* fifo */
                    
                    #define 	S_BLKSIZE  1024 /* size of a block */
                    
                    #define	    S_ISUID		0004000	/* set user id on execution */
                    #define	    S_ISGID		0002000	/* set group id on execution */
                    
                    #ifndef	_POSIX_SOURCE
                        #define	    S_ISVTX		0001000	/* save swapped text even after use */
                        #define	    S_IREAD		0000400	/* read permission, owner */
                        #define	    S_IWRITE 	0000200	/* write permission, owner */
                        #define	    S_IEXEC		0000100	/* execute/search permission, owner */
                        #define	    S_ENFMT 	0002000	/* enforcement-mode locking */
                        
                        #define	    S_IFMT		_IFMT
                        #define	    S_IFDIR		_IFDIR
                        #define	    S_IFCHR		_IFCHR
                        #define	    S_IFBLK		_IFBLK
                        #define	    S_IFREG		_IFREG
                        #define	    S_IFLNK		_IFLNK
                        #define	    S_IFSOCK	_IFSOCK
                        #define	    S_IFIFO		_IFIFO
                    #endif	/* !_POSIX_SOURCE */
                    
                    
                    #define	S_IRWXU 	(S_IRUSR | S_IWUSR | S_IXUSR)
                    #define		S_IRUSR	0000400	/* read permission, owner */
                    #define		S_IWUSR	0000200	/* write permission, owner */
                    #define		S_IXUSR 0000100/* execute/search permission, owner */
                    #define	S_IRWXG		(S_IRGRP | S_IWGRP | S_IXGRP)
                    #define		S_IRGRP	0000040	/* read permission, group */
                    #define		S_IWGRP	0000020	/* write permission, grougroup */
                    #define		S_IXGRP 0000010/* execute/search permission, group */
                    #define	S_IRWXO		(S_IROTH | S_IWOTH | S_IXOTH)
                    #define		S_IROTH	0000004	/* read permission, other */
                    #define		S_IWOTH	0000002	/* write permission, other */
                    #define		S_IXOTH 0000001/* execute/search permission, other */
                    
                    #define	S_ISBLK(m)	(((m)&_IFMT) == _IFBLK)
                    #define	S_ISCHR(m)	(((m)&_IFMT) == _IFCHR)
                    #define	S_ISDIR(m)	(((m)&_IFMT) == _IFDIR)
                    #define	S_ISFIFO(m)	(((m)&_IFMT) == _IFIFO)
                    #define	S_ISREG(m)	(((m)&_IFMT) == _IFREG)
                    #define	S_ISLNK(m)	(((m)&_IFMT) == _IFLNK)
                    #define	S_ISSOCK(m)	(((m)&_IFMT) == _IFSOCK)
                    
                    
                    
                    int	   chmod  ( const char *__path, mode_t __mode );
                    int    fchmod ( int __fd, mode_t __mode);
                    
                    int	   fstat  ( int VFH_Handle, struct stat *Stat );
                    int	   stat   ( const char *NameFile, struct stat *Stat );
                    int	   lstat  ( const char *NameFile, struct stat *Stat );
                    
                    mode_t umask  ( mode_t __mask );
                    
                    #ifdef __cplusplus
                    }
                    #endif
                    
                   
#endif   // ifndef _NANOC_SYS_STAT_H_
