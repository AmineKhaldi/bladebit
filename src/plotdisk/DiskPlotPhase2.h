#pragma once

#include "plotdisk/DiskPlotContext.h"

template<uint32 _numBuckets>
struct DiskPairAndMapReader;

class DiskPlotPhase2
{
public:
    DiskPlotPhase2( DiskPlotContext& context );
    ~DiskPlotPhase2();

    void Run();

private:

    template<uint32 _numBuckets>
    void RunWithBuckets();

    template<uint32 _numBuckets>
    void    MarkTable( const TableId rTable, DiskPairAndMapReader<_numBuckets> reader, Pair* pairs, uint64* map, uint64* lTableMarks, uint64* rTableMarks );

    template<TableId table, uint32 _numBuckets>
    void    MarkTableBuckets( DiskPairAndMapReader<_numBuckets> reader, Pair* pairs, uint64* map, uint64* lTableMarks, uint64* rTableMarks );

private:
    DiskPlotContext& _context;
    Fence*           _bucketReadFence;
    Fence*           _mapWriteFence;
    size_t           _markingTableSize = 0;
};