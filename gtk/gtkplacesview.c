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

#include <gio/gio.h>
#include <gtk/gtk.h>

#include "gtkintl.h"
#include "gtkplacesview.h"
#include "gtktypebuiltins.h"

/*
 * PlacesRow class definition
 */
#define PLACES_TYPE_ROW (places_row_get_type ())
GDK_AVAILABLE_IN_3_18
G_DECLARE_FINAL_TYPE (PlacesRow, places_row, PLACES, ROW, GtkListBoxRow)

struct _GtkPlacesViewPrivate
{
  GVolumeMonitor                *volume_monitor;

  GtkWidget                     *drives_listbox;

  guint local_only             : 1;
};

struct _PlacesRow
{
  GtkListBoxRow parent_instance;
  GtkImage *icon_image;
  GtkLabel *name_label;
  GtkLabel *path_label;
};

G_DEFINE_TYPE (PlacesRow, places_row, GTK_TYPE_LIST_BOX_ROW)

G_DEFINE_TYPE_WITH_PRIVATE (GtkPlacesView, gtk_places_view, GTK_TYPE_BOX)

enum {
  PROP_ICON = 1,
  PROP_NAME,
  PROP_PATH,
  NUM_PROPS
};

enum {
  PROP_0,
  PROP_LOCAL_ONLY,
  LAST_PROP
};

static GParamSpec *properties [LAST_PROP];
static GParamSpec *row_properties [NUM_PROPS];

static void
places_row_get_property (GObject    *object,
                         guint       prop_id,
                         GValue     *value,
                         GParamSpec *pspec)
{
  PlacesRow *self = PLACES_ROW (object);
  GIcon *icon = NULL;

  switch (prop_id)
    {
    case PROP_ICON:
      gtk_image_get_gicon (self->icon_image, &icon, NULL);
      g_value_set_object (value, icon);
      break;

    case PROP_NAME:
      g_value_set_string (value, gtk_label_get_label (self->name_label));
      break;

    case PROP_PATH:
      g_value_set_string (value, gtk_label_get_label (self->path_label));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
places_row_set_property (GObject      *object,
                         guint         prop_id,
                         const GValue *value,
                         GParamSpec   *pspec)
{
  PlacesRow *self = PLACES_ROW (object);

  switch (prop_id)
    {
    case PROP_ICON:
      gtk_image_set_from_gicon (self->icon_image,
                                g_value_get_object (value),
                                GTK_ICON_SIZE_LARGE_TOOLBAR);
      break;

    case PROP_NAME:
      gtk_label_set_label (self->name_label, g_value_get_string (value));
      break;

    case PROP_PATH:
      gtk_label_set_label (self->path_label, g_value_get_string (value));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
places_row_class_init (PlacesRowClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->get_property = places_row_get_property;
  object_class->set_property = places_row_set_property;

  row_properties[PROP_ICON] =
          g_param_spec_object ("icon",
                               P_("Icon of the row"),
                               P_("The icon representing the volume"),
                               G_TYPE_ICON,
                               G_PARAM_READWRITE);

  row_properties[PROP_NAME] =
          g_param_spec_string ("name",
                               P_("Name of the volume"),
                               P_("The name of the volume"),
                               "",
                               G_PARAM_READWRITE);

  row_properties[PROP_PATH] =
          g_param_spec_string ("path",
                               P_("Path of the volume"),
                               P_("The path of the volume"),
                               "",
                               G_PARAM_READWRITE);

  g_object_class_install_properties (object_class, NUM_PROPS, row_properties);

  gtk_widget_class_set_template_from_resource (widget_class, "/org/gtk/libgtk/ui/placesrow.ui");

  gtk_widget_class_bind_template_child (widget_class, PlacesRow, icon_image);
  gtk_widget_class_bind_template_child (widget_class, PlacesRow, name_label);
  gtk_widget_class_bind_template_child (widget_class, PlacesRow, path_label);
}

static void
places_row_init (PlacesRow *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
}

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

  g_clear_object (&priv->volume_monitor);

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
gtk_places_view_constructed (GObject *object)
{
  GtkPlacesViewPrivate *priv = GTK_PLACES_VIEW (object)->priv;
  GList *mounts;
  GList *l;

  if (G_OBJECT_CLASS (gtk_places_view_parent_class)->constructed)
    G_OBJECT_CLASS (gtk_places_view_parent_class)->constructed (object);

  mounts = g_volume_monitor_get_mounts (priv->volume_monitor);

  for (l = mounts; l != NULL; l = l->next)
    {
      GtkWidget *row;
      GMount *mount;
      GDrive *drive;
      GFile *root;
      GIcon *icon;
      gchar *name;
      gchar *path;

      mount = l->data;
      drive = g_mount_get_drive (mount);

      /* Removable devices won't appear here */
      if (drive && g_drive_is_media_removable (drive))
        {
          g_object_unref (drive);
          continue;
        }

      root = g_mount_get_root (mount);
      icon = g_mount_get_icon (mount);
      name = g_mount_get_name (mount);
      path = g_file_get_path (root);

      row = g_object_new (PLACES_TYPE_ROW,
                          "icon", icon,
                          "name", name,
                          "path", path,
                          NULL);

      gtk_list_box_insert (GTK_LIST_BOX (priv->drives_listbox), row, -1);

      g_clear_object (&root);
      g_clear_object (&icon);
      g_clear_object (&drive);
      g_free (name);
      g_free (path);
    }

  g_list_free_full (mounts, g_object_unref);
}

static void
gtk_places_view_class_init (GtkPlacesViewClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  klass->set_local_only = gtk_places_view_real_set_local_only;
  klass->get_local_only = gtk_places_view_real_get_local_only;

  object_class->finalize = gtk_places_view_finalize;
  object_class->constructed = gtk_places_view_constructed;
  object_class->get_property = gtk_places_view_get_property;
  object_class->set_property = gtk_places_view_set_property;

  properties[PROP_LOCAL_ONLY] =
          g_param_spec_boolean ("local-only",
                                P_("Local Only"),
                                P_("Whether the sidebar only includes local files"),
                                FALSE,
                                G_PARAM_READWRITE);

  g_object_class_install_properties (object_class, LAST_PROP, properties);

  /* Bind class to template */
  gtk_widget_class_set_template_from_resource (widget_class, "/org/gtk/libgtk/ui/gtkplacesview.ui");

  gtk_widget_class_bind_template_child_private (widget_class, GtkPlacesView, drives_listbox);
}

static void
gtk_places_view_init (GtkPlacesView *self)
{
  self->priv = gtk_places_view_get_instance_private (self);
  self->priv->volume_monitor = g_volume_monitor_get ();

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
 * gtk_places_view_get_local_only:
 * @view: a #GtkPlacesView
 *
 * Returns %TRUE if only local volumes are shown, i.e. no networks
 * are displayed.
 *
 * Returns: %TRUE if only local volumes are shown, %FALSE otherwise.
 *
 * Since: 3.18
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

/**
 * gtk_places_view_set_local_only:
 * @view: a #GtkPlacesView
 * @local_only: %TRUE to hide remote locations, %FALSE to show.
 *
 * Sets the #GtkPlacesView::local-only property to @local_only.
 *
 * Returns:
 *
 * Since: 3.18
 */
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
