/* gtkplacesview.c
 *
 * Copyright (C) 2015 Georges Basile Stavracas Neto <georges.stavracas@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include "gtkplacesview.h"

struct _GtkPlacesViewPrivate
{
  guint local_only             : 1;
};

G_DEFINE_TYPE_WITH_PRIVATE (GtkPlacesView, gtk_places_view, GTK_TYPE_BOX)

enum {
  PROP_0,
  PROP_LOCAL_ONLY,
  LAST_PROP
};

static GParamSpec *properties [LAST_PROP];

static void
gtk_places_view_finalize (GObject *object)
{
  GtkPlacesView *self = (GtkPlacesView *)object;
  GtkPlacesViewPrivate *priv = gtk_places_view_get_instance_private (self);

  G_OBJECT_CLASS (gtk_places_view_parent_class)->finalize (object);
}

static void
gtk_places_view_get_property (GObject    *object,
                              guint       prop_id,
                              GValue     *value,
                              GParamSpec *pspec)
{
  GtkPlacesView *self = GTK_PLACES_VIEW (object);

  switch (prop_id)
    {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gtk_places_view_set_property (GObject      *object,
                              guint         prop_id,
                              const GValue *value,
                              GParamSpec   *pspec)
{
  GtkPlacesView *self = GTK_PLACES_VIEW (object);

  switch (prop_id)
    {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gtk_places_view_class_init (GtkPlacesViewClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = gtk_places_view_finalize;
  object_class->get_property = gtk_places_view_get_property;
  object_class->set_property = gtk_places_view_set_property;
}

static void
gtk_places_view_init (GtkPlacesView *self)
{
}

/**
 * gtk_places_view_new:
 *
 * Creates a new #GtkPlacesView widget.
 *
 * The application should connect to at least the
 * #GtkPlacesSidebar::open-location signal to be notified
 * when the user makes a selection in the sidebar.
 *
 * Returns: a newly created #GtkPlacesView
 *
 * Since: 3.18
 */
GtkWidget *
gtk_places_view_new (void)
{
  return g_object_new (GTKPLACESVIEW_TYPE_, NULL);
}
