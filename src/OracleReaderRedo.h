/* Header for OracleReaderRedo class
   Copyright (C) 2018-2020 Adam Leszczynski.

This file is part of Open Log Replicator.

Open Log Replicator is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as published
by the Free Software Foundation; either version 3, or (at your option)
any later version.

Open Log Replicator is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
Public License for more details.

You should have received a copy of the GNU General Public License
along with Open Log Replicator; see the file LICENSE.txt  If not see
<http://www.gnu.org/licenses/>.  */

#include "types.h"
#include "RedoLogRecord.h"

#ifndef ORACLEREADERREDO_H_
#define ORACLEREADERREDO_H_

using namespace std;

#define VECTOR_MAX_LENGTH 512
#define REDO_END                0x0008
#define REDO_ASYNC              0x0100
#define REDO_NODATALOSS         0x0200
#define REDO_RESYNC             0x0800
#define REDO_CLOSEDTHREAD       0x1000
#define REDO_MAXPERFORMANCE     0x2000

namespace OpenLogReplicator {

    class OracleReader;
    class OpCode;

    class OracleReaderRedo {
    private:
        OracleReader *oracleReader;
        int64_t group;
        uint64_t blockSize;
        typeblk blockNumber;
        typeblk numBlocks;
        uint64_t lastBytesRead;
        bool lastReadSuccessfull;
        bool headerInfoPrinted;
        int64_t fileDes;
        typescn lastCheckpointScn;
        typescn extScn;
        typescn curScn;
        typescn curScnPrev;
        typesubscn curSubScn;
        uint64_t recordBeginPos;
        typeblk recordBeginBlock;
        typetime recordTimestmap;
        uint64_t recordPos;
        uint64_t recordLeftToCopy;
        uint64_t redoBufferPos;
        uint64_t redoBufferFileStart;
        uint64_t redoBufferFileEnd;

        void initFile();
        uint64_t readFile();
        uint64_t checkBlockHeader(uint8_t *buffer, typeblk blockNumber);
        uint64_t checkRedoHeader();
        uint64_t processBuffer();
        void analyzeRecord();
        void flushTransactions(typescn checkpointScn);
        void appendToTransaction(RedoLogRecord *redoLogRecord);
        void appendToTransaction(RedoLogRecord *redoLogRecord1, RedoLogRecord *redoLogRecord2);
        typesum calcChSum(uint8_t *buffer, uint64_t size);

    public:
        string path;
        typescn firstScn;
        typescn nextScn;
        typeseq sequence;

        void reload();
        void clone(OracleReaderRedo *redo);
        uint64_t processLog();
        OracleReaderRedo(OracleReader *oracleReader, int64_t group, const char* path);
        virtual ~OracleReaderRedo();

        friend ostream& operator<<(ostream& os, const OracleReaderRedo& ors);
    };
}

#endif
