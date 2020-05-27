/*
 *  globals.h
 *  mdieditor
 *
 *  Created by bazz on 2/28/12.
 *  Copyright 2012 S.Crew. All rights reserved.
 *
 */
#ifndef GLOBALS_H
#define GLOBALS_H
#include <QtWidgets>
/*#include <QFile>
#include <QTextStream>
#include <QCursor>*/

#define MODE_SWAP 0
#define MODE_MOVE 1

#define SIZE_8x8 1
#define SIZE_16x16 2
#define SIZE_32x32 4
#define SIZE_64x64 8

extern int twidth, theight, rows, cols;
extern QTextStream debug;
extern QFile debugfile;
extern QCursor m_Cursor;
//extern int tileWHLSize, tileHHLSize;	// tile width highlight size, hight highlight size
#endif
