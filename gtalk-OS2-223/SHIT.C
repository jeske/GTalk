
int g_getfree(int drive, unsigned long int far *freebytes,
            unsigned long int far *totalbytes)
{
  int b = _DS;
  struct jumptable far *jump = jmptl;
  struct dfree dbuf;
  int islocked;

  _ES = old_ES;
  _DS = old_DS;
  islocked = (jump->islocked)(DOS_SEM);
  if (!islocked) (jump->lock_dos)(1001);
  (jump->getdfree)(drive,&dbuf);
  if (!islocked) (jump->unlock_dos)();
  _ES = _DS = b;
  if (dbuf.df_sclus == 0xFFFF) return (-1);
  *freebytes = ((unsigned long int)dbuf.df_avail) *
               ((unsigned long int)dbuf.df_bsec) *
               ((unsigned long int)dbuf.df_sclus);
  *totalbytes = ((unsigned long int)dbuf.df_total) *
               ((unsigned long int)dbuf.df_bsec) *
               ((unsigned long int)dbuf.df_sclus);
  return (0);
}
