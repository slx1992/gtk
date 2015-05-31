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
  GVolumeMonitor                *volume_monitor;

  guint local_only             : 1;
};

G_DEFINE_TYPE_WITH_PRIVATE (GtkPlacesView, gtk_places_view, GTK_TYPE_BOX)

enum {
  PROP_0,
  PROP_LOCAL_ONLY,
  LAST_PROP
};

static GParamSpec *properties [LAST_PROP];

static gboolean
gtk_places_view_real_get_local_only (GtkPlacesView *view)
{
  return view->priv->local_only;
}

static void
gtk_places_view_real_set_local_only (GtkPlacesView *view,
                                     gboolean       local_only)
{
  if (view->priv->local_only != local_only)
    {
      view->priv->local_only = local_only;

      /* TODO: implement me */

      g_object_notify_by_pspec (G_OBJECT (view), properties [PROP_LOCAL_ONLY]);
    }
}

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
    case PROP_LOCAL_ONLY:
      g_value_set_boolean (value, gtk_places_view_get_local_only (self));
      break;

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
    case PROP_LOCAL_ONLY:
      gtk_places_view_set_local_only (self, g_value_get_boolean (value));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gtk_places_view_class_init (GtkPlacesViewClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  klass->set_local_only = gtk_places_view_real_set_local_only;
  klass->get_local_only = gtk_places_view_real_get_local_only;

  object_class->finalize = gtk_places_view_finalize;
  object_class->get_property = gtk_places_view_get_property;
  object_class->set_property = gtk_places_view_set_property;

  /* Bind class to template */
  gtk_widget_class_set_template_from_resource (widget_class, "/org/gtk/libgtk/ui/gtkplacesview.ui");
}

static void
gtk_places_view_init (GtkPlacesView *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
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
  return g_object_new (GTK_TYPE_PLACES_VIEW, NULL);
}

/**
 * gtkplacesview_func:
 *
 * %TRUE if only local volumes are shown, %FALSE otherwise.
 */
gboolean
gtk_places_view_get_local_only (GtkPlacesView *view)
{
  GtkPlacesViewClass *class;

  g_return_val_if_fail (GTK_IS_PLACES_VIEW (view), FALSE);

  class = GTK_PLACES_VIEW_GET_CLASS (view);

  g_assert (class->get_local_only != NULL);
  return class->get_local_only (view);
}

void
gtk_places_view_set_local_only (GtkPlacesView *view,
                                gboolean       local_only)
{
  GtkPlacesViewClass *class;

  g_return_if_fail (GTK_IS_PLACES_VIEW (view));

  class = GTK_PLACES_VIEW_GET_CLASS (view);

  g_assert (class->get_local_only != NULL);
  class->set_local_only (view, local_only);
}
