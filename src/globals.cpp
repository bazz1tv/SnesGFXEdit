/*
 *  globals.cpp
 *  mdieditor
 *
 *  Created by bazz on 2/28/12.
 *  Copyright 2012 S.Crew. All rights reserved.
 *
 */
#include "globals.h"

//int tileWHLSize=8, tileHHLSize=8;
int twidth=8, theight=8;
int rows=32, cols=16;

QFile debugfile("/Users/bazz/debug.txt");
//debugfile.open(QIODevice::WriteOnly);
QTextStream debug;
QCursor m_Cursor;
//debugstream.setIntegerBase(16);
//debugstream<<"Debug Information\n";
