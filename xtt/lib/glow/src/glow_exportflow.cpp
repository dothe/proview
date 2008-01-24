/* 
 * Proview   $Id: glow_exportflow.cpp,v 1.1 2008-01-17 14:17:05 claes Exp $
 * Copyright (C) 2005 SSAB Oxel�sund AB.
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
 * along with the program, if not, write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 **/

#include "glow_std.h"


#include <iostream.h>
#include <float.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include "glow_growctx.h"
#include "glow_growrect.h"
#include "glow_growline.h"
#include "glow_growconpoint.h"
#include "glow_growtext.h"
#include "glow_growsubannot.h"
#include "glow_growarc.h"
#include "glow_exportflow.h"
#include "flow.h"

#define FLOW_SCALE 0.05

int GlowExportFlow::export_flow( char *filename)
{
  char		nc_name[80];
  char		*s;

  if ( (s = strrchr( filename, ':')))
    strcpy( nc_name, s+1);
  else if ( (s = strrchr( filename, '/')))
    strcpy( nc_name, s+1);
  else
    strcpy( nc_name, filename);
  if ( (s = strrchr( nc_name, '.')))
    *s = 0;
  fp.open( filename);

  // Save as NodeClass
  fp <<	int(flow_eSave_NodeClass) << endl;
  fp <<	int(flow_eSave_NodeClass_nc_name) << FSPACE << nc_name << endl;
  fp <<	int(flow_eSave_NodeClass_a) << endl;
  ctx->a.export_flow( this);
  fp <<	int(flow_eSave_NodeClass_group) << FSPACE << int(glow_eNodeGroup_Common) << endl;
  fp << int(flow_eSave_NodeClass_no_con_obstacle) << FSPACE << ctx->no_con_obstacle << endl;
  fp <<	int(flow_eSave_End) << endl;

  fp.close();
  return 1;
}

void GlowExportFlow::array( GlowArray *o)
{
  int i;

  fp <<	int(flow_eSave_Array) << endl;
  for ( i = 0; i < o->a_size; i++)
  {
    if ( o->a[i]->type() != glow_eObjectType_Con)
      o->a[i]->export_flow( this);
  }
  fp <<	int(flow_eSave_End) << endl;
}

void GlowExportFlow::rect( GrowRect *o)
{
  flow_eDrawType draw_type = flow_eDrawType_Line;

  if ( o->draw_type >= 20 && o->draw_type < 60)
    draw_type = flow_eDrawType_LineGray;

  fp << int(flow_eSave_Rect) << endl;
  fp << int(flow_eSave_Rect_draw_type) << FSPACE << int(draw_type) << endl;
  fp << int(flow_eSave_Rect_line_width) << FSPACE << o->line_width << endl;
  fp << int(flow_eSave_Rect_display_level) << FSPACE << int(o->display_level) << endl;
  fp << int(flow_eSave_Rect_ll) << endl;
  point( &o->ll, &o->trf);
  fp << int(flow_eSave_Rect_ur) << endl;
  point( &o->ur, &o->trf);
  fp << int(flow_eSave_End) << endl;
}

void GlowExportFlow::line( GrowLine *o)
{
  flow_eDrawType draw_type = flow_eDrawType_Line;

  if ( o->draw_type >= 20 && o->draw_type < 60)
    draw_type = flow_eDrawType_LineGray;

  fp << int(flow_eSave_Line) << endl;
  fp << int(flow_eSave_Line_draw_type) << FSPACE << int(draw_type) << endl;
  fp << int(flow_eSave_Line_line_width) << FSPACE << o->line_width << endl;
  fp << int(flow_eSave_Line_p1) << endl;
  point( &o->p1, &o->trf);
  fp << int(flow_eSave_Line_p2) << endl;
  point( &o->p2, &o->trf);
  fp << int(flow_eSave_End) << endl;
}

void GlowExportFlow::conpoint( GrowConPoint *o)
{
  fp <<	int(flow_eSave_ConPoint) << endl;
  fp <<	int(flow_eSave_ConPoint_number) << FSPACE << o->number << endl;
  fp <<	int(flow_eSave_ConPoint_direction) << FSPACE << int(o->direction) << endl;
  fp <<	int(flow_eSave_ConPoint_p) << endl;
  point( &o->p, &o->trf);
  fp << int(flow_eSave_ConPoint_trace_attribute) << FSPACE << o->trace_attribute << endl;
  fp << int(flow_eSave_ConPoint_trace_attr_type) << FSPACE << int(o->trace_attr_type) << endl;
  fp <<	int(flow_eSave_End) << endl;
}

void GlowExportFlow::text( GrowText *o)
{
  flow_eDrawType draw_type;

  switch ( o->draw_type) {
  case glow_eDrawType_TextHelveticaBold:
    draw_type = flow_eDrawType_TextHelveticaBold;
    break;
  default:
    draw_type = flow_eDrawType_TextHelvetica;    
  }

  fp << int(flow_eSave_Text) << endl;
  fp << int(flow_eSave_Text_text_size) << FSPACE << o->text_size << endl;
  fp << int(flow_eSave_Text_draw_type) << FSPACE << int(draw_type) << endl;
  fp << int(flow_eSave_Text_text) << FSPACE << o->text << endl;
  fp << int(flow_eSave_Text_p) << endl;
  point( &o->p, &o->trf);
  fp << int(flow_eSave_End) << endl;
}

void GlowExportFlow::annot( GrowSubAnnot *o)
{
  flow_eDrawType draw_type;

  switch ( o->draw_type) {
  case glow_eDrawType_TextHelveticaBold:
    draw_type = flow_eDrawType_TextHelveticaBold;
    break;
  default:
    draw_type = flow_eDrawType_TextHelvetica;    
  }

  fp << int(flow_eSave_Annot) << endl;
  fp << int(flow_eSave_Annot_number) << FSPACE << o->number << endl;
  fp << int(flow_eSave_Annot_draw_type) << FSPACE << int(draw_type) << endl;
  fp << int(flow_eSave_Annot_text_size) << FSPACE << o->text_size << endl;
  fp << int(flow_eSave_Annot_display_level) << FSPACE << int(o->display_level) << endl;
  fp << int(flow_eSave_Annot_p) << endl;
  point( &o->p, &o->trf);
  fp << int(flow_eSave_Annot_annot_type) << FSPACE << int(o->annot_type) << endl;
  fp << int(flow_eSave_End) << endl;
}

void GlowExportFlow::arc( GrowArc *o)
{
  flow_eDrawType draw_type = flow_eDrawType_Line;

  if ( o->draw_type >= 20 && o->draw_type < 60)
    draw_type = flow_eDrawType_LineGray;

  fp << int(flow_eSave_Arc) << endl;
  fp << int(flow_eSave_Arc_draw_type) << FSPACE << int(draw_type) << endl;
  fp << int(flow_eSave_Arc_line_width) << FSPACE << o->line_width << endl;
  fp << int(flow_eSave_Arc_angel1) << FSPACE << o->angel1 << endl;
  fp << int(flow_eSave_Arc_angel2) << FSPACE << o->angel2 << endl;
  fp << int(flow_eSave_Arc_ll) << endl;
  point( &o->ll, &o->trf);
  fp << int(flow_eSave_Arc_ur) << endl;
  point( &o->ur, &o->trf);
  fp << int(flow_eSave_End) << endl;
}

void GlowExportFlow::point( GlowPoint *o, GlowTransform *trf)
{
  double x = trf->x( o->x, o->y) * FLOW_SCALE;
  double y = trf->y( o->x, o->y) * FLOW_SCALE;

  fp << int(flow_eSave_Point) << endl;
  fp << int(flow_eSave_Point_x) << FSPACE << x << endl;
  fp << int(flow_eSave_Point_y) << FSPACE << y << endl;
  fp << int(flow_eSave_End) << endl;
}
