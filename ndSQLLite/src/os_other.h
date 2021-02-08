#ifndef _SQLITE_OS_OTHER_H_
#define _SQLITE_OS_OTHER_H_

#if OS_ND_PSP
#define sqlite3OsOpenReadWrite      sqlite3ndOpenReadWrite
#define sqlite3OsOpenExclusive      sqlite3ndOpenExclusive
#define sqlite3OsOpenReadOnly       sqlite3ndOpenReadOnly
#define sqlite3OsDelete             sqlite3ndDelete
#define sqlite3OsFileExists         sqlite3ndFileExists
#define sqlite3OsFullPathname       sqlite3ndFullPathname
#define sqlite3OsIsDirWritable      sqlite3ndIsDirWritable
#define sqlite3OsSyncDirectory      sqlite3ndSyncDirectory
#define sqlite3OsTempFileName       sqlite3ndTempFileName
#define sqlite3OsRandomSeed         sqlite3ndRandomSeed
#define sqlite3OsSleep              sqlite3ndSleep
#define sqlite3OsCurrentTime        sqlite3ndCurrentTime
#define sqlite3OsEnterMutex         sqlite3ndEnterMutex
#define sqlite3OsLeaveMutex         sqlite3ndLeaveMutex
#define sqlite3OsInMutex            sqlite3ndInMutex
#define sqlite3OsThreadSpecificData sqlite3ndThreadSpecificData
#define sqlite3OsMalloc             sqlite3GenericMalloc
#define sqlite3OsRealloc            sqlite3GenericRealloc
#define sqlite3OsFree               sqlite3GenericFree
#define sqlite3OsAllocationSize     sqlite3GenericAllocationSize
#define sqlite3OsDlopen             sqlite3ndDlopen
#define sqlite3OsDlsym              sqlite3ndDlsym
#define sqlite3OsDlclose            sqlite3ndDlclose
#endif

#endif /* _SQLITE_OS_OTHER_H_ */
