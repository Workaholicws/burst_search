/*
 * As far as I can tell there is only one parameter in Jon's code, a *depth*
 * parameter in `map_chans`.  *delta_t* and *max_dm* are only required to
 * interpret `depth`. I may be completely wrong on this. -km
 *
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <omp.h>

#include "dedisperse_gbt.h"
#include "dedisperse.h"


// For allocating output buffer.
size_t burst_get_num_dispersions(size_t nfreq, float freq0,
                           float delta_f, int depth) {
  //return 10;
  return get_nchan_from_depth(depth);
}

// Return minimum *depth* parameter required to achieve given maximum DM.
int burst_depth_for_max_dm(float max_dm, size_t nfreq, float freq0,
                            float delta_f, float delta_t) {
  int depth=2;
  int imax=20;
  int i=0;
  while ((get_diagonal_dm_simple(freq0,freq0+nfreq*delta_f,delta_t,depth)<max_dm)&&(i<imax)) {
    depth++;
    i++;
  }
  if (i==imax) {
    fprintf(stderr,"Failure in burst_depth_for_max_dm.  Did not reach requested DM of %12.4g at a depth of %d\n",max_dm,i);
    return 0;
  }

    return depth;
}


//JLS 08 Aug 2014
//since the silly way I have of mapping frequency channels into lambda^2 channels is a bit slow
//but is also static, pre-calculate it and put the mapping into chan_map.
//chan_map should be pre-allocated, with at least 2**depth elements

void setup_channel_mapping(size_t nfreq, float freq0, float delta_f,int depth, int *chan_map) 
{
  int nchan=get_nchan_from_depth(depth);
  int i,j;

  float l0=1.0/(freq0+0.5*delta_f);
  l0=l0*l0;
  float l1=1.0/(freq0+(nfreq-0.5)*delta_f);
  l1=l1*l1;
  if (l0>l1) {
    float tmp=l0;
    l0=l1;
    l1=tmp;
  }

  float *out_chans=(float *)malloc(sizeof(float)*nchan);
  float dl=(l1-l0)/(nchan-1);
  for (i=0;i<nchan;i++)
    out_chans[i]=l0+dl*i;
  
  for (i=0;i<nfreq;i++) {
    float myl=1.0/(freq0+(0.5+i)*i);
    myl=myl*myl;
    int jmin=-1;
    float minerr=1e30;
    for (j=0;j<nchan;j++) {
      float curerr=fabs(myl-out_chans[j]);
      if (curerr<minerr) {
	minerr=curerr;
	jmin=j;
      }
    }
    chan_map[i]=jmin;
  }
  
  
}


// *ntime2* is allowed to be 0.  Return number of valid dedispersed time samples,
// (always less than, and usually equal to, *ntime1*).  *delta_f* may be negative.
// Frequencies are in Hz.
size_t burst_dm_transform(float *indata1, float *indata2, float *outdata,
                  size_t nfreq, float freq0, float delta_f,
			  size_t ntime1, size_t ntime2, float delta_t, int depth) {
  

  //return my_burst_dm_transform(indata1,indata2,outdata,ntime1,ntime2,delta_t,nfreq,chan_map,depth);

  return 8;
}
