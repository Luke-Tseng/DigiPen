int is_palindrome2(const char *phrase, int length) {
    int i;
    int j;
    for(i = 0; i< length;++i) {
        j = *(phrase +length-1-i); /* set j to last char - i */
        if(*(phrase+i) != j) { /* if it doesn't match, return 0 */
            return 0;
        }
    }
    return 1; /* return 1 if it goes through the whole loop */
}