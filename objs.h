#define NS_VAR     0		/* unfielded var and typedef namespace	*/
#define NS_COMPLEX 1		/* namespace for struct, etc. tags	*/
#define NS_TYPEDEF 2		/* used only by ident() and declare()	*/
#define NS_HALFDEC 3		/* used only by dec1/2tag and declare()	*/
#define NS_ANYTYPE 4		/* can be assigned any process type	*/
#define NS_NULLPID 5		/* can be assigned to any process	*/
#define NS_BADTAG  6		/* illegal namespace, for bad tag	*/

#define e_basic	  0		/* values for obj field in typespec	*/
#define e_struct  1
#define e_union	  2
#define e_process 3
#define e_unsigned	4	/* New ones to do full typing of basic types */
#define e_unsignedint	5
#define e_unsignedchar	6
#define e_unsignedshort	7
#define e_unsignedlong	8
#define e_longint	9
#define e_double	10
#define e_short		11
#define e_char		12
#define e_float		13
#define e_int		14

