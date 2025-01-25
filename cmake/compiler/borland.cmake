
add_compiler_flag(-w-)              # disable all warnings by default

macro(enable_warnings)
    add_compiler_flag_release(-w!)  # treat warnings as errors
    add_compiler_flag(-wamb)        # ambiguous operators need parentheses
    add_compiler_flag(-wamp)        # superfluous & with function
    #add_compiler_flag(-wasc)        # restarting compile using assembly
    add_compiler_flag(-wasm)        # unknown assembler instruction
    add_compiler_flag(-waus)        # variable is assigned a value that is never used
    #add_compiler_flag(-wali)        # incorrect use of #pragma alias "aliasName"="substituteName"
    #add_compiler_flag(-watr)        # attribute directive ignored
    #add_compiler_flag(-wadt)        # cannot allocate arrays of Delphi style class
    add_compiler_flag(-wbbf)        # bit fields must be signed or unsigned int
    add_compiler_flag(-wbei)        # initialization with a different type
    add_compiler_flag(-wbig)        # hexadecimal value contains too many digits
    #add_compiler_flag(-wbex)        # base class of dllexport class should also be dllexport
    #add_compiler_flag(-wbcx)        # base class of exported class should have exported constructor
    add_compiler_flag(-wccc)        # condition is always true/false
    add_compiler_flag(-wcln)        # constant is long
    #add_compiler_flag(-wcom)        # continuation character \ found in // comment
    add_compiler_flag(-wcpt)        # nonportable pointer comparison
    #add_compiler_flag(-wcsu)        # comparing signed and unsigned values
    #add_compiler_flag(-wcod)        # incorrect use of #pragma codeseg
    #add_compiler_flag(-wcni)        # constant in new expression requires an initializer
    add_compiler_flag(-wdef)        # possible use of symbol before definition
    #add_compiler_flag(-wdig)        # declaration ignored
    add_compiler_flag(-wdpu)        # declare symbol prior to use in prototype
    add_compiler_flag(-wdsz)        # array size for 'delete' ignored
    add_compiler_flag(-wdup)        # redefinition of '%s' is not identical
    #add_compiler_flag(-wdep)        # use of deprecated syntax
    #add_compiler_flag(-wdtp)        # parameter is a dependent type
    #add_compiler_flag(-wdpr)        # accessing deprecated entity 
    #add_compiler_flag(-wdex)        # unresolved dependencies in expression
    #add_compiler_flag(-wdlx)        # function defined with different linkage
    #add_compiler_flag(-wdim)        # can't import a function being defined
    #add_compiler_flag(-wdgu)        # interface does not have a GUID
    #add_compiler_flag(-wdiu)        # interface does not derive from IUnknown
    #add_compiler_flag(-wdin)        # casting Delphi style class to an interface
    #add_compiler_flag(-wdns)        # support for declspec not implemented.
    add_compiler_flag(-weas)        # assignment of incompatible types
    add_compiler_flag(-weff)        # code has no effect
    add_compiler_flag(-wext)        # symbol is declared as both external and static
    #add_compiler_flag(-wexc)        # extern C linkage ignored
    #add_compiler_flag(-weoi)        # dllexport overrides previous dllimport
    add_compiler_flag(-whch)        # handler hidden by previous handler
    add_compiler_flag(-whid)        # function hides virtual function
    #add_compiler_flag(-wias)        # array variable is near
    add_compiler_flag(-wibc)        # base class is also a base class of other class
    add_compiler_flag(-will)        # ill-formed pragma
    add_compiler_flag(-winl)        # functions are not expanded inline
    #add_compiler_flag(-wifr)        # function redefined as non-inline
    #add_compiler_flag(-wimp)        # implicit conversion of type to type
    #add_compiler_flag(-wiex)        # inline function was declared with 'extern template'
    #add_compiler_flag(-winc)        # constant expression expected; statement ignored
    #add_compiler_flag(-wiip)        # inline member function in Package class
    #add_compiler_flag(-wiac)        # found invalid character from source code in the current text locale
    #add_compiler_flag(-wind)        # casting interface to Delphi style class
    #add_compiler_flag(-witl)        # initialization of TLS data is not supported on this platform
    add_compiler_flag(-wlin)        # temporary used as initializer
    add_compiler_flag(-wlvc)        # temporary used for parameter
    add_compiler_flag(-wmpc)        # conversion will fail for members of virtual base
    add_compiler_flag(-wmpd)        # maximum precision used for member pointer type
    add_compiler_flag(-wmsg)        # #pragma message
    #add_compiler_flag(-wmes)        # incorrect use of #pragma message
    #add_compiler_flag(-wmcs)        # incorrect use of #pragma code_seg
    #add_compiler_flag(-wmcc)        # multi-character character constant
    #add_compiler_flag(-wmls)        # local Static with constructor dangerous for multi-threaded apps
    #add_compiler_flag(-wmnc)        # member in class without constructors
    #add_compiler_flag(-wmal)        # alignment reduced
    #add_compiler_flag(-wmtx)        # dllexport class member should be of exported type
    add_compiler_flag(-wnak)        # non-ANSI keyword used
    add_compiler_flag(-wncf)        # non-const function called for const object
    add_compiler_flag(-wnci)        # constant member is not initialized
    #add_compiler_flag(-wncl)        # constructor initializer list ignored
    #add_compiler_flag(-wnfd)        # function body ignored
    #add_compiler_flag(-wngu)        # negating unsigned value
    #add_compiler_flag(-wnin)        # initializer for object ignored
    #add_compiler_flag(-wnma)        # macro definition ignored
    #add_compiler_flag(-wnmu)        # #undef directive ignored
    add_compiler_flag(-wnod)        # no declaration for function
    add_compiler_flag(-wnsf)        # declaration of static function ignored
    add_compiler_flag(-wnst)        # use qualified name to access member type
    add_compiler_flag(-wntd)        # use '> >' for nested templates instead of '>>'
    #add_compiler_flag(-wnto)        # no type OBJ file present; disabling external types option
    add_compiler_flag(-wnvf)        # non-volatile function called for volatile object
    #add_compiler_flag(-wnpp)        # pragma pack pop with no matching pack push
    #add_compiler_flag(-wntn)        # type name expected
    #add_compiler_flag(-wnrm)        # published method refers to an unpublishable parameter or return type
    add_compiler_flag(-wobi)        # published method refers to an unpublishable parameter or return type
    add_compiler_flag(-wobs)        # obsolete
    add_compiler_flag(-wofp)        # style of function definition is now obsolete
    #add_compiler_flag(-wosh)        # possible overflow in shift operation
    #add_compiler_flag(-wovf)        # integer arithmetic overflow
    #add_compiler_flag(-wonr)        # not all options can be restored at this time
    add_compiler_flag(-wovl)        # 
    add_compiler_flag(-wpar)        # parameter is never used
    add_compiler_flag(-wpch)        # cannot create pre-compiled header
    #add_compiler_flag(-wpck)        # structure packing size has changed
    add_compiler_flag(-wpia)        # possibly incorrect assignment
    add_compiler_flag(-wpin)        # initialization is only partially bracketed
    #add_compiler_flag(-wpow)        # previous options and warnings not restored
    add_compiler_flag(-wpre)        # overloaded prefix operator used as a postfix operator
    add_compiler_flag(-wpro)        # call to function with no prototype
    #add_compiler_flag(-wprc)        # suggest parentheses to clarify precedence
    #add_compiler_flag(-wpcm)        # incorrect use of #pragma comment
    #add_compiler_flag(-wpsb)        # static `main' is not treated as an entry point
    #add_compiler_flag(-wptl)        # path and filename exceed maximum size
    #add_compiler_flag(-wpnf)        # path not found
    #add_compiler_flag(-wpun)        # unknown #pragma ignored
    #add_compiler_flag(-wpns)        # pragma not supported on this platform.
    add_compiler_flag(-wrch)        # unreachable code
    add_compiler_flag(-wret)        # both return and return with a value used
    add_compiler_flag(-wrng)        # constant out of range in comparison
    add_compiler_flag(-wrpt)        # nonportable pointer conversion
    add_compiler_flag(-wrvl)        # function should return a value
    #add_compiler_flag(-wrlo)        # returning pointer to a local object
    add_compiler_flag(-wsig)        # conversion may lose significant digits
    #add_compiler_flag(-wspa)        # suspicious pointer arithmetic
    #add_compiler_flag(-wstu)        # undefined structure 
    add_compiler_flag(-wstv)        # structure passed by value
    add_compiler_flag(-wsus)        # suspicious pointer conversion
    #add_compiler_flag(-wstl)        # operator must be publicly visible / iterator required
    #add_compiler_flag(-wsmx)        # requested savemem exceeds amount available
    #add_compiler_flag(-wtai)        # template instance is already instantiated
    #add_compiler_flag(-wtes)        # explicitly specializing an explicitly specialized class member makes no sense
    #add_compiler_flag(-wthr)        # throw expression violates exception specification
    #add_compiler_flag(-wtlb)        # deprecated #import directive encountered. Please use the TLIBIMP utility instead
    add_compiler_flag(-wucp)        # mixing pointers to different 'char' types
    add_compiler_flag(-wuse)        # symbol is declared but never used
    #add_compiler_flag(-wucn)        # character cannot be represented in the current ansi locale
    add_compiler_flag(-wvoi)        # void functions may not return a value
    add_compiler_flag(-wzdi)        # division by zero
endmacro()
