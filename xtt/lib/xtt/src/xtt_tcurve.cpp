/* 
 * Proview   Open Source Process Control.
 * Copyright (C) 2005-2012 SSAB EMEA AB.
 *
 * This file is part of Proview.
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with Proview. If not, see <http://www.gnu.org/licenses/>
 *
 * Linking Proview statically or dynamically with other modules is
 * making a combined work based on Proview. Thus, the terms and 
 * conditions of the GNU General Public License cover the whole 
 * combination.
 *
 * In addition, as a special exception, the copyright holders of
 * Proview give you permission to, from the build function in the
 * Proview Configurator, combine Proview with modules generated by the
 * Proview PLC Editor to a PLC program, regardless of the license
 * terms of these modules. You may copy and distribute the resulting
 * combined work under the terms of your choice, provided that every 
 * copy of the combined work is accompanied by a complete copy of 
 * the source code of Proview (the version used to produce the 
 * combined work), being distributed under the terms of the GNU 
 * General Public License plus this exception.
 */

#include "flow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "pwr.h"
#include "pwr_baseclasses.h"
#include "rt_gdh.h"
#include "rt_gdh_msg.h"
#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"
#include "cow_wow.h"
#include "rt_sev_msg.h"

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "glow_growctx.h"
#include "glow_growapi.h"
#include "glow_curvectx.h"
#include "glow_curveapi.h"

#include "xtt_xnav.h"
#include "xtt_tcurve.h"
#include "ge_curve.h"


XttTCurve::XttTCurve( void *parent_ctx,
		      const char *name,
		      pwr_tAttrRef *xn_arefv,
		      int *sts) :
  xnav(parent_ctx), gcd(0), curve(0), rows(0), vsize(0), timerid(0), close_cb(0), help_cb(0), 
  get_select_cb(0), first_scan(1), time_low_old(0), time_high_old(0)
{
  pwr_tTime from, to;

  memset( &tc, 0, sizeof(tc));

  if ( xn_arefv == 0 || xn_arefv[0].Objid.vid == 0) {
    aref_cnt = 0;
    gcd = new GeCurveData( curve_eDataType_DsTrend);
    return;
  }

  // Count number of curves
  for ( aref_cnt = 0; xn_arefv[aref_cnt].Objid.vid != 0; aref_cnt++) ;

  if ( aref_cnt > XTT_TCURVE_MAX)
    aref_cnt = XTT_TCURVE_MAX;

  memcpy( arefv, xn_arefv, aref_cnt * sizeof(arefv[0]));

  time_Period( time_ePeriod_OneMinute, &from, &to, 0, 0);
  if ( aref_cnt == 1) {
    get_data( sts, from, to);
    if ( EVEN(*sts)) return;
  }
  else {
    get_multidata( sts, from, to);
    if ( EVEN(*sts)) return;
  }
  cdh_StrncpyCutOff( title, name, sizeof(title), 1);
}

XttTCurve::~XttTCurve()
{
}



int XttTCurve::get_data( pwr_tStatus *sts, pwr_tTime from, pwr_tTime to)
{
  pwr_tDeltaTime trange;
  int from_idx, to_idx;
  pwr_tDeltaTime diff;
  pwr_tFloat32 timerange;
  int interval;
  

  load_data( sts, &arefv[0]);
  if ( EVEN(*sts))
    return 0;

  // Calculate interval
  if ( time_Acomp( &from, &tc.last_time) > 0 ||
       time_Acomp( &to, &tc.first_time) < 0)
    // No samples in this interval
    return 0;

  time_Adiff( &diff, &tc.last_time, &tc.first_time);
  timerange = time_DToFloat( 0, &diff);
  if ( time_Acomp( &from, &tc.first_time) < 0) {
    from = tc.first_time;
    from_idx = 0;
  }
  else {
    time_Adiff( &diff, &from, &tc.first_time);
    from_idx = time_DToFloat( 0, &diff) * tc.timebuf_samples / timerange;
  }

  if ( time_Acomp( &to, &tc.last_time) >= 0)
    to_idx = tc.timebuf_samples;
  else {
    time_Adiff( &diff, &tc.last_time, &to);
    to_idx = tc.timebuf_samples - time_DToFloat( 0, &diff) * tc.timebuf_samples / timerange;
  }

  interval = (to_idx - from_idx) / 1000 + 1;

  rows = (to_idx - from_idx) / interval;
  if ( from_idx + rows * interval > tc.timebuf_samples)
    rows = (tc.timebuf_samples - from_idx) / interval;

  // Create data for time axis
  gcd = new GeCurveData( curve_eDataType_DsTrend);

  gcd->x_data[0] = (double *) calloc( 1, 8 * rows);
  if ( tc.timeelement_size == 4) {
    for ( int i = 0; i < rows; i++)
      gcd->x_data[0][i] = (double) (*((unsigned int *)tc.tbuf + from_idx + i * interval));
  }
  else {
    for ( int i = 0; i < rows; i++)
      gcd->x_data[0][i] = (double)(*((unsigned int *)tc.tbuf + 2*from_idx + 2*i*interval)) + 
	(double)1e-9 * (*((unsigned int *)tc.tbuf + 2*from_idx + 2*i*interval + 1));
  }

  strcpy( gcd->x_name, "Time");
  gcd->x_axis_type[0] = curve_eAxis_x;
  strcpy( gcd->x_format[0], "%10t");


  for ( int j = 0; j < tc.bufcnt; j++) {

    strncpy( gcd->y_name[j], tc.name[j], sizeof(gcd->y_name[0]));
    gcd->rows[j] = rows;
    
    gcd->y_data[j] = (double *) calloc( 1, 8 * rows);

    for ( int i = 0; i < rows; i++) {
      if ( i >= tc.buf_samples[j])
	break;
      switch ( tc.type[j]) {
      case pwr_eType_Int64:
	gcd->y_data[j][i] = *((pwr_tInt32 *)tc.vbuf[j] + from_idx + i*interval);
	break;
      case pwr_eType_Int32:
	gcd->y_data[j][i] = *((pwr_tInt32 *)tc.vbuf[j] + from_idx + i*interval);
	break;
      case pwr_eType_Int16:
	gcd->y_data[j][i] = *((pwr_tInt32 *)tc.vbuf[j] + from_idx + i*interval);
	break;
      case pwr_eType_Int8:
	gcd->y_data[j][i] = *((pwr_tInt32 *)tc.vbuf[j] + from_idx + i*interval);
	break;
      case pwr_eType_UInt64:
	gcd->y_data[j][i] = *((pwr_tUInt32 *)tc.vbuf[j] + from_idx + i*interval);
	break;
      case pwr_eType_UInt32:
	gcd->y_data[j][i] = *((pwr_tUInt32 *)tc.vbuf[j] + from_idx + i*interval);
	break;
      case pwr_eType_UInt16:
	gcd->y_data[j][i] = *((pwr_tUInt32 *)tc.vbuf[j] + from_idx + i*interval);
	break;
      case pwr_eType_UInt8:
	gcd->y_data[j][i] = *((pwr_tUInt32 *)tc.vbuf[j] + from_idx + i*interval);
	break;
      case pwr_eType_Float32:
	gcd->y_data[j][i] = *((pwr_tFloat32 *)tc.vbuf[j] + from_idx + i*interval);
	break;
      case pwr_eType_Float64:
	gcd->y_data[j][i] = *((pwr_tFloat64 *)tc.vbuf[j] + from_idx + i*interval);
	break;
      case pwr_eType_Boolean:
	gcd->y_data[j][i] = *((pwr_tBoolean *)tc.vbuf[j] + from_idx + i*interval);
	break;
      default: 
	*sts = SEV__CURVETYPE;
	return 0;
    }
    }
  }

  gcd->y_axis_type[0] = curve_eAxis_y;

  gcd->cols = tc.bufcnt;

  gcd->get_borders();
  gcd->get_default_axis();

  if ( to.tv_sec != 0 && from.tv_sec != 0) {
    time_Adiff( &trange, &to, &from);
    if ( time_DToFloat( 0, &trange) < 600)
      strcpy( gcd->x_format[0], "%10t");
    else
      strcpy( gcd->x_format[0], "%11t");
  }
  else
    strcpy( gcd->x_format[0], "%11t");
    

  gcd->select_color( 0);

  if ( curve) {
    curve->set_curvedata( gcd);  // This will free the old gcd 
    curve->configure_curves();
    curve->configure_axes();
    curve->redraw();
  }
  *sts = SEV__SUCCESS;
  return 1;
}

int XttTCurve::get_multidata( pwr_tStatus *sts, pwr_tTime from, pwr_tTime to)
{
#if 0
  pwr_tTime *tbuf;
  void *vbuf;
  pwr_tDeltaTime trange;
  int curve_cnt = 0;

  // Create data for time axis
  gcd = new GeCurveData( curve_eDataType_MultiTrend);
  strcpy( gcd->x_name, "Time");

  for ( int k = 0; k < aref_cnt; k++) {

    if ( curve_cnt >= CURVE_MAX_COLS) {
      printf("To many columns for curve class max: %d\n", CURVE_MAX_COLS);
      break;
    }
      
    // TCurveObject object
    int numAttributes;

    get_selected_data( sts, from, to, 1000, &tbuf, &vbuf);
    if ( EVEN(*sts))
      return 0;
      
    if ( rows == 0)
      continue;

      gcd->x_data[curve_cnt] = (double *) calloc( 1, 8 * rows);
      for ( int i = 0; i < rows; i++)
	gcd->x_data[curve_cnt][i] = (double)tbuf[i].tv_sec + (double)1e-9 * tbuf[i].tv_nsec;
      
      strcpy( gcd->x_name, "Time");
      gcd->x_axis_type[curve_cnt] = curve_eAxis_x;
      strcpy( gcd->x_format[0], "%10t");
      
      //todo linesize b�r vi kunna f� fr�n sevcli_get_objectitemdata
      int linesize = 0;
      for ( int i = 0; i < numAttributes; i++) {
	linesize += histattrbuf[i].size;
      }
      void *dataptr = vbuf;
      int tmp = 0;
      for ( int i = 0; i < numAttributes; i++) {
	gcd->x_data[curve_cnt] = (double *) calloc( 1, 8 * rows);
	for ( int j = 0; j < rows; j++)
	  gcd->x_data[curve_cnt][j] = (double)tbuf[j].tv_sec + (double)1e-9 * tbuf[j].tv_nsec;
	
	strcpy( gcd->x_name, "Time");
	gcd->x_axis_type[curve_cnt] = curve_eAxis_x;
	strcpy( gcd->x_format[curve_cnt], "%10t");
	
	switch ( histattrbuf[i].type ) {
	case pwr_eType_Int64:
	case pwr_eType_Int32:
	case pwr_eType_Int16:
	case pwr_eType_Int8:
	case pwr_eType_UInt64:
	case pwr_eType_UInt32:
	case pwr_eType_UInt16:
	case pwr_eType_UInt8:
	case pwr_eType_Float32:
	case pwr_eType_Float64:
	case pwr_eType_Boolean:
	  break;
	default: 
	  tmp += histattrbuf[i].size;
	  continue;
	}
	
	if ( curve_cnt >= CURVE_MAX_COLS) {
	  printf("To many columns for curve class max:%d\n", CURVE_MAX_COLS);
	  break;
	}
	
	strcpy( gcd->y_name[curve_cnt], "todo");
	gcd->y_data[curve_cnt] = (double *) calloc( 1, 8 * rows);
	gcd->y_axis_type[curve_cnt] = curve_eAxis_y;
	
	dataptr = (char *)vbuf + tmp;
	for ( int j = 0; j < rows; j++) {
	  dataptr = ((char *)vbuf)+ j * linesize + tmp;
	  switch ( histattrbuf[i].type ) {
	  case pwr_eType_Int64:
	    gcd->y_data[curve_cnt][j] = *(pwr_tInt64 *)dataptr;
	    break;
	  case pwr_eType_Int32:
	    gcd->y_data[curve_cnt][j] = *(pwr_tInt32 *)dataptr;
	    break;
	  case pwr_eType_Int16:
	    gcd->y_data[curve_cnt][j] = *(pwr_tInt16 *)dataptr;
	    break;
	  case pwr_eType_Int8:
	    gcd->y_data[curve_cnt][j] = *(pwr_tInt8 *)dataptr;
	    break;
	  case pwr_eType_UInt64:
	    gcd->y_data[curve_cnt][j] = *(pwr_tUInt64 *)dataptr;
	    break;
	  case pwr_eType_UInt32:
	    gcd->y_data[curve_cnt][j] = *(pwr_tUInt32 *)dataptr;
	    break;
	  case pwr_eType_UInt16:
	    gcd->y_data[curve_cnt][j] = *(pwr_tUInt16 *)dataptr;
	    break;
	  case pwr_eType_UInt8:
	    gcd->y_data[curve_cnt][j] = *(pwr_tUInt8 *)dataptr;
	    break;
	  case pwr_eType_Float32:
	    gcd->y_data[curve_cnt][j] = *(pwr_tFloat32 *)dataptr;
	    break;
	  case pwr_eType_Float64:
	    gcd->y_data[curve_cnt][j] = *(pwr_tFloat64 *)dataptr;
	    break;
	  case pwr_eType_Boolean:
	    gcd->y_data[curve_cnt][j] = *(pwr_tBoolean *)dataptr;
	    break;
	  default: 
	    *sts = SEV__CURVETYPE;
	    return 0;
	  }
	}
	tmp += histattrbuf[i].size;
	gcd->y_axis_type[curve_cnt] = curve_eAxis_y;

	gcd->rows[curve_cnt] = rows;
	curve_cnt++;
      }
      free( tbuf);
      free( vbuf);
      free( histattrbuf);
    }
  }

  if( curve_cnt == 0 ) {
    delete gcd;
    *sts = SEV__NODATATIME;
    return 0;
  }

  gcd->cols = curve_cnt;

  gcd->get_borders();
  gcd->get_default_axis();

  if ( to.tv_sec != 0 && from.tv_sec != 0) {
    time_Adiff( &trange, &to, &from);
    if ( time_DToFloat( 0, &trange) < 600)
      strcpy( gcd->x_format[0], "%10t");
    else
      strcpy( gcd->x_format[0], "%11t");
  }
  else
    strcpy( gcd->x_format[0], "%11t");
    

  gcd->select_color( 0);

  if ( curve) {
    curve->set_curvedata( gcd);  // This will free the old gcd 
    curve->configure_curves();
    curve->configure_axes();
    curve->redraw();
  }
  *sts = SEV__SUCCESS;
#endif
  return 1;
}

void XttTCurve::curve_add( pwr_tAttrRef aref)
{
  if ( aref_cnt == XTT_TCURVE_MAX)
    return;

  if ( gcd->type != curve_eDataType_MultiTrend) {
    // Convert to multidata

    for ( int i = 1; i < gcd->cols; i++) {
      gcd->rows[i] = gcd->rows[0];
      gcd->x_data[i] = (double *) calloc( 1, 8 * gcd->rows[i]);
      memcpy( gcd->x_data[i], gcd->x_data[0], 8 * gcd->rows[i]);

      gcd->x_axis_type[i] = curve_eAxis_x;
      strcpy( gcd->x_format[i], "%10t");

    }
    gcd->type = curve_eDataType_MultiTrend;
  }
  
  arefv[aref_cnt] = aref;
  aref_cnt++;

  int curve_cnt = gcd->cols;
  rows = 0;

  gcd->x_data[curve_cnt] = (double *) calloc( 1, 8 * rows);
  
  gcd->x_axis_type[curve_cnt] = curve_eAxis_x;
  strcpy( gcd->x_format[curve_cnt], "%10t");

  strcpy( gcd->y_name[curve_cnt], "");
  gcd->y_data[curve_cnt] = (double *) calloc( 1, 8 * rows);
  
  gcd->cols++;

  gcd->select_color( 0);
  curve->config_names();
}


void XttTCurve::pop()
{
  curve->pop();
}

void XttTCurve::tcurve_close_cb( void *ctx)
{
  XttTCurve *tcurve = (XttTCurve *) ctx;

  if ( tcurve->close_cb)
    (tcurve->close_cb)( tcurve->xnav, tcurve);
  else
    delete tcurve;
}

void XttTCurve::tcurve_decrease_period_cb( void *ctx)
{
  XttTCurve *tcurve = (XttTCurve *) ctx;
  time_ePeriod period;
  pwr_tStatus sts;
  int changed;

  sts = tcurve->curve->get_period( &period);

  changed = time_PeriodZoomIn( &period);
  if ( changed)
    tcurve->curve->set_period( period, 0);

}

void XttTCurve::tcurve_increase_period_cb( void *ctx)
{
  XttTCurve *tcurve = (XttTCurve *) ctx;
  time_ePeriod period;
  pwr_tStatus sts;
  int changed;

  sts = tcurve->curve->get_period( &period);

  changed = time_PeriodZoomOut( &period);
  if ( changed)
    tcurve->curve->set_period( period, 0);

}

void XttTCurve::tcurve_reload_cb( void *ctx)
{
  XttTCurve *tcurve = (XttTCurve *) ctx;
  pwr_tTime t_low, t_high;
  pwr_tStatus sts;

  sts = tcurve->curve->get_times( &t_low, &t_high);
  if ( EVEN(sts)) {
    tcurve->wow->DisplayError( "Time", "Time syntax error");
    return;
  }

  if ( time_Acomp( &t_high, &t_low) != 1) {
    tcurve->wow->DisplayError( "Time", "Start time later than end time");
    return;
  }

  if ( t_low.tv_sec < 0)
    t_low.tv_sec = 0;

  if ( tcurve->aref_cnt == 1)
    tcurve->get_data( &sts, t_low, t_high);
  else
    tcurve->get_multidata( &sts, t_low, t_high);

  tcurve->time_low_old = 0;
  tcurve->time_high_old = 0;
  tcurve->curve->set_center_from_window( 1);
}

void XttTCurve::tcurve_prev_period_cb( void *ctx)
{
  XttTCurve *tcurve = (XttTCurve *) ctx;
  pwr_tTime from, to, prev_from, prev_to;
  pwr_tStatus sts;
  time_ePeriod period;

  sts = tcurve->curve->get_times( &prev_from, &prev_to);
  if ( EVEN(sts)) {
    tcurve->wow->DisplayError( "Time", "Time syntax error");
    return;
  }

  sts = tcurve->curve->get_period( &period);

  if ( time_Acomp( &prev_to, &prev_from) != 1) {
    tcurve->wow->DisplayError( "Time", "Start time later than end time");
    return;
  }

  time_PreviousPeriod( period, &prev_from, &prev_to, &from, &to);

  tcurve->curve->set_times( &from, &to);

  int change_period = 1;
  switch( period) {
  case time_ePeriod_LastMinute:
    period = time_ePeriod_OneMinute;
    break;
  case time_ePeriod_Last10Minutes:
    period = time_ePeriod_10Minutes;
    break;
  case time_ePeriod_LastHour:
    period = time_ePeriod_OneHour;
    break;
  case time_ePeriod_Today:
    period = time_ePeriod_Yesterday;
    break;
  case time_ePeriod_Yesterday:
    period = time_ePeriod_OneDay;
    break;
  case time_ePeriod_ThisWeek:
    period = time_ePeriod_LastWeek;
    break;
  case time_ePeriod_LastWeek:
    period = time_ePeriod_OneWeek;
    break;
  case time_ePeriod_ThisMonth:
    period = time_ePeriod_LastMonth;
    break;
  case time_ePeriod_LastMonth:
    period = time_ePeriod_OneMonth;
    break;
  case time_ePeriod_ThisYear:
    period = time_ePeriod_OneYear;
    break;
  default:
    change_period = 0;
  }
  if ( change_period)
    tcurve->curve->set_period( period, 1);
    
}

void XttTCurve::tcurve_next_period_cb( void *ctx)
{
  XttTCurve *tcurve = (XttTCurve *) ctx;
  pwr_tTime from, to, prev_from, prev_to;
  pwr_tStatus sts;
  time_ePeriod period;

  sts = tcurve->curve->get_times( &prev_from, &prev_to);
  if ( EVEN(sts)) {
    tcurve->wow->DisplayError( "Time", "Time syntax error");
    return;
  }

  sts = tcurve->curve->get_period( &period);

  if ( time_Acomp( &prev_to, &prev_from) != 1) {
    tcurve->wow->DisplayError( "Time", "Start time later than end time");
    return;
  }
 
  time_NextPeriod( period, &prev_from, &prev_to, &from, &to);

  tcurve->curve->set_times( &from, &to);

}

void XttTCurve::tcurve_add_cb( void *ctx)
{
  XttTCurve *tcurve = (XttTCurve *) ctx;
  pwr_tOid oid;
  pwr_tOName aname, oname;
  int sts;

  if ( !tcurve->get_select_cb)
    return;

  sts = tcurve->get_select_cb( tcurve->xnav, &oid, aname, oname);
  if ( EVEN(sts)) return;

  pwr_tAttrRef aref = cdh_ObjidToAref(oid);
  tcurve->curve_add( aref);
}

void XttTCurve::tcurve_remove_cb( void *ctx)
{
  // Do do
}

int XttTCurve::tcurve_export_cb( void *ctx, pwr_tTime *from, pwr_tTime *to, int rows, int idx, 
				   char *filename)
{
#if 0
  XttTCurve *tcurve = (XttTCurve *) ctx;
  pwr_tFileName fname;
  pwr_tTime *tbuf;
  void *vbuf;
  pwr_eType	vtype;
  unsigned int  vsize;
  pwr_tStatus sts;
  int rrows;
  char timestr[40];
  FILE *fp;

  dcli_translate_filename( fname, filename);


  sevcli_get_itemdata( &sts, tcurve->scctx, tcurve->oidv[idx], tcurve->anamev[idx], *from, *to, 
		       rows, &tbuf, &vbuf, &rrows, &vtype, &vsize);
  if ( EVEN(sts))
    return sts;

  if( rrows == 0 ) {
    return SEV__NODATATIME;
  }

  fp = fopen( fname, "w");
  if ( !fp)
    return SEV__EXPORTFILE;

  for ( int i = 0; i < rrows; i++) {
    time_AtoAscii( &tbuf[i], time_eFormat_DateAndTime, timestr, sizeof(timestr));
    fprintf( fp, "%s, ", timestr);
    switch ( vtype) {
    case pwr_eType_Int32:
    case pwr_eType_Int64:
    case pwr_eType_Int16:
    case pwr_eType_Int8:
      fprintf( fp, "%d", ((pwr_tInt32 *)vbuf)[i]);
      break;
    case pwr_eType_UInt64:
    case pwr_eType_UInt32:
    case pwr_eType_UInt16:
    case pwr_eType_UInt8:
      fprintf( fp, "%u", ((pwr_tUInt32 *)vbuf)[i]);
      break;
    case pwr_eType_Float32:
      fprintf( fp, "%g", ((pwr_tFloat32 *)vbuf)[i]);
      break;
    case pwr_eType_Float64:
      fprintf( fp, "%g", ((pwr_tFloat64 *)vbuf)[i]);
      break;
    case pwr_eType_Boolean:
      fprintf( fp, "%d", ((pwr_tBoolean *)vbuf)[i]);
      break;
    default: 
      sts = SEV__CURVETYPE;
    }
    fprintf( fp, "\n");
  }
  free( tbuf);
  free( vbuf);
  fclose( fp);

  printf( "Exported %d rows to file \"%s\" (%d)\n", rrows, fname, idx);

  if ( EVEN(sts))
    return sts;
#endif
  return SEV__SUCCESS;
}

void XttTCurve::tcurve_help_cb( void *ctx)
{
  XttTCurve *tcurve = (XttTCurve *) ctx;

  if ( tcurve->help_cb)
    (tcurve->help_cb)( tcurve->xnav, "opg_trendbuffer");
}

void XttTCurve::tcurve_scan( void *data)
{
  XttTCurve *tcurve = (XttTCurve *) data;

  if ( tcurve->timerid)
    tcurve->timerid->add( 1000, tcurve_scan, tcurve);
}


int XttTCurve::load_data( pwr_tStatus *sts, pwr_tAttrRef *aref)
{
  pwr_tTid tid;

  if ( tc.vbuf[0] == 0) {
    // Fetch buffer data

    *sts = gdh_GetAttrRefTid( aref, &tid);
    if ( EVEN(*sts)) return 0;

    switch ( tid) {
    case pwr_cClass_DsTrendCurve: {
      pwr_sClass_DsTrendCurve trend;
      pwr_sClass_CircBuffHeader head;
      pwr_tAttrRef head_aref;
      int idx;
      int first_idx;
      int last_idx;
      int size;
      char *tb;
      char *vb[10];
      int header_size = pwr_AlignLW(sizeof(pwr_sClass_CircBuffHeader));
      
      *sts = gdh_GetObjectInfoAttrref( aref, &trend, sizeof(trend));
      if ( EVEN(*sts)) return 0;

      idx = 0;
      for ( int j = 0; j < 10; j++) {
	if ( cdh_ObjidIsNull( trend.Buffers[j].Objid))
	  continue;

	*sts = gdh_AttrrefToName( &trend.Attribute[j], tc.name[idx], sizeof(tc.name[0]), cdh_mNName);
	if ( EVEN(*sts)) return 0;

	tc.buf_aref[idx] = trend.Buffers[j];
	tc.time_resolution = trend.TimeResolution;

	*sts = gdh_ArefANameToAref( &tc.buf_aref[idx], "Head", &head_aref);
	if ( EVEN(*sts)) continue;

	*sts = gdh_GetObjectInfoAttrref( &head_aref, &head, sizeof(head));
	if ( EVEN(*sts)) return 0;

	tc.buf_size[idx] = head.Size;
	tc.element_size[idx] = head.ElementSize;
	tc.type[idx] = (pwr_eType) trend.AttributeType[j];
	tc.buf_bsize[idx] = tc.buf_size[idx] * head.ElementSize + header_size;
	vb[idx] = (char *)calloc( 1, tc.buf_bsize[idx]);

	idx++;
      }
      tc.bufcnt = idx;
      if ( idx == 0)
	return 0;

      // Time buffer
      if ( cdh_ObjidIsNull( trend.TimeBuffer.Objid))
	return 0;

      tc.timebuf_aref = trend.TimeBuffer;

      *sts = gdh_ArefANameToAref( &tc.timebuf_aref, "Head", &head_aref);
      if ( EVEN(*sts)) return 0;

      *sts = gdh_GetObjectInfoAttrref( &head_aref, &head, sizeof(head));
      if ( EVEN(*sts)) return 0;

      tc.timebuf_size = head.Size;
      tc.timeelement_size = head.ElementSize;
      tc.timebuf_bsize = tc.timebuf_size * head.ElementSize + header_size;
      tb = (char *)calloc( 1, tc.timebuf_bsize);

      // Get buffer data
      for ( int j = 0; j < tc.bufcnt; j++) {
	*sts = gdh_GetObjectInfoAttrref( &tc.buf_aref[j], vb[j], tc.buf_bsize[j]);
	if ( EVEN(*sts)) return 0;
      }

      *sts = gdh_GetObjectInfoAttrref( &tc.timebuf_aref, tb, tc.timebuf_bsize);
      if ( EVEN(*sts)) return 0;

      // TODO, Check size and first/last index
      last_idx = ((pwr_sClass_CircBuffHeader *)tb)->LastIndex;
      first_idx = ((pwr_sClass_CircBuffHeader *)tb)->FirstIndex;

      tc.first_time.tv_sec = *(unsigned int *) (tb + header_size + tc.timeelement_size * first_idx);
      tc.last_time.tv_sec = *(unsigned int *) (tb + header_size + tc.timeelement_size * last_idx);
      if ( tc.timeelement_size == 8) {
	tc.first_time.tv_nsec = *(unsigned int *) (tb + header_size + tc.timeelement_size * first_idx + 4);
	tc.last_time.tv_nsec = *(unsigned int *) (tb + header_size + tc.timeelement_size * last_idx + 4);
      }
      else {
	tc.first_time.tv_nsec = 0;
	tc.last_time.tv_nsec = 0;
      }
      
      if ( first_idx == last_idx)
	return 0;
      if ( first_idx < last_idx) {
	size = last_idx - first_idx + 1;
	tc.tbuf = (char *)calloc( size, tc.timeelement_size);
	memcpy( tc.tbuf, tb + header_size + tc.timeelement_size * first_idx, size * tc.timeelement_size);
      }      
      else {
	size = last_idx + 1 + (tc.timebuf_size - first_idx);
	tc.tbuf = (char *)calloc( size, tc.timeelement_size);
	memcpy( tc.tbuf, tb + header_size + tc.timeelement_size * first_idx, (tc.timebuf_size - first_idx) * tc.timeelement_size);
	memcpy( tc.tbuf + (tc.timebuf_size - first_idx) * tc.timeelement_size, tb + header_size,
		first_idx * tc.timeelement_size);
      }
      tc.timebuf_samples = size;

      for ( int j = 0; j < tc.bufcnt; j++) {
	last_idx = ((pwr_sClass_CircBuffHeader *)vb[j])->LastIndex;
	first_idx = ((pwr_sClass_CircBuffHeader *)vb[j])->FirstIndex;
      
	if ( first_idx == last_idx)
	  return 0;
	if ( first_idx < last_idx) {
	  size = last_idx - first_idx + 1;
	  tc.vbuf[j] = (char *)calloc( size, tc.element_size[j]);
	  memcpy( tc.vbuf[j], vb[j] + header_size + tc.element_size[j] * first_idx, size * tc.element_size[j]);
	}      
	else {
	  size = last_idx + 1 + (tc.buf_size[j] - first_idx);
	  tc.vbuf[j] = (char *)calloc( size, tc.element_size[j]);
	  memcpy( tc.vbuf[j], vb[j] + header_size + tc.element_size[j] * first_idx, 
		  (tc.buf_size[j] - first_idx) * tc.element_size[j]);
	  memcpy( tc.vbuf[j] + (tc.buf_size[j] - first_idx) * tc.element_size[j], vb[j] + header_size, first_idx * tc.element_size[j]);
	}	
	tc.buf_samples[j] = size;
      }
      free( tb);
      for ( int j = 0; j < tc.bufcnt; j++)
	free( vb[j]);


      break;
    }
    default:
      return 0;
    }
  }
  return 1;
}



