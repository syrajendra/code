Dejagnu
=======
DejaGnu always produces a summary output file such as tool.sum.

This tool parses such file to produce a csv file.

    Usage: 
    ./parse_summary_file.py <summary-output-file.sum>
    Optional arguments: 
    --with-ttype : Extra column showing test type/description
    --no-redundant-filename : Repeated filenames are discarded
