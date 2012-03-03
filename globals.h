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
#include <QFile>
#include <QTextStream>
#include <QCursor>

extern int twidth, theight, rows, cols;
extern QTextStream debug;
extern QFile debugfile;
extern QCursor m_Cursor;
//extern int tileWHLSize, tileHHLSize;	// tile width highlight size, hight highlight size
#endif