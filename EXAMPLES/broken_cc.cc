/* broken_cc.cc */

/*
 *  Here are two problems I don't intend to fix for a while.
 *  First, cc complains about floatarray having null dimension,
 *  which ccc has no way of catching.
 *  Second, ccc does not catch mismatch in # of params for process
 *  specs and bodies, but it generates code that makes cc die.
 */
typedef float floatarray[][];

process spec noparam();

process body noparam(a)
{
}

main()
{
}
