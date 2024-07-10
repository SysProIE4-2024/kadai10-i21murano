/*
 *  mysystem.c : system 関数のクローン
 */

#include <stdio.h>
#include <stdlib.h>    // exit, system のため
#include <unistd.h>    // fork, execXX のため
#include <sys/wait.h>  // wait のため
#include "mysystem.h"  // インタフェース

// system関数のクローン
int mysystem(char *command) {
  //NULLポインタを渡された場合は1を返す
  if(command == NULL){
    return 1;
  }
  int status = 0;
  int pid = fork();
  //forkできなかった場合は-1を返す
  if(pid < 0){
    return -1;
  }
  //それ以外では/bin/shの終了ステータスを返す
  if(pid != 0){//親プロセス
    while(wait(&status) != pid){//子プロセスが終了するのを待つ
      ;
    }
  } else{//子プロセス
    execl("/bin/sh", "sh", "-c", command, NULL);
    perror("/bin/sh");//　/bin/shの起動に失敗時の終了ステータスは127
    exit(127);
  }
  return status;
}

/* 実行例

% make
cc -Wall -std=c99 -o mysysmain mysysmain.c  mysystem.c

% ls -l
total 616
-rw-r--r--  1 yunosuke301  staff     143  7  6 22:42 Makefile
-rw-r--r--  1 yunosuke301  staff    2795  7  6 22:42 README.md
-rw-r--r--  1 yunosuke301  staff  238232  7  6 22:42 README.pdf
-rwxr-xr-x  1 yunosuke301  staff   50358  7 10 21:59 mysysmain
-rw-r--r--  1 yunosuke301  staff     925  7  6 22:42 mysysmain.c
-rw-r--r--  1 yunosuke301  staff     952  7 10 21:59 mysystem.c
-rw-r--r--  1 yunosuke301  staff      90  7  6 22:42 mysystem.h

% ./mysysmain "ls -l"
mysystem:                                   <--- ここからmysystemの結果
total 616
-rw-r--r--  1 yunosuke301  staff     143  7  6 22:42 Makefile
-rw-r--r--  1 yunosuke301  staff    2795  7  6 22:42 README.md
-rw-r--r--  1 yunosuke301  staff  238232  7  6 22:42 README.pdf
-rwxr-xr-x  1 yunosuke301  staff   50358  7 10 21:59 mysysmain
-rw-r--r--  1 yunosuke301  staff     925  7  6 22:42 mysysmain.c
-rw-r--r--  1 yunosuke301  staff    1470  7 10 22:00 mysystem.c
-rw-r--r--  1 yunosuke301  staff      90  7  6 22:42 mysystem.h
retval = 00000000
system:                                     <--- ここからsystemの結果
total 616
-rw-r--r--  1 yunosuke301  staff     143  7  6 22:42 Makefile
-rw-r--r--  1 yunosuke301  staff    2795  7  6 22:42 README.md
-rw-r--r--  1 yunosuke301  staff  238232  7  6 22:42 README.pdf
-rwxr-xr-x  1 yunosuke301  staff   50358  7 10 21:59 mysysmain
-rw-r--r--  1 yunosuke301  staff     925  7  6 22:42 mysysmain.c
-rw-r--r--  1 yunosuke301  staff    1470  7 10 22:00 mysystem.c
-rw-r--r--  1 yunosuke301  staff      90  7  6 22:42 mysystem.h
retval = 00000000

% ./mysysmain                               <--- 引数がない場合
使い方 : ./mysysmain コマンド文字列

*/
