void assert(bool x)
{
        if (!x)
                __coverity_panic__();
}
