/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphic.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ellanglo <ellanglo@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 15:04:51 by ellanglo          #+#    #+#             */
/*   Updated: 2025/12/01 16:03:44 by ellanglo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <Application.h>
#include <curl/curl.h>
#include <gtk/gtk.h>
#include <defines.h>
#include <stdio.h>
#include <http.h>

static void app_open(GtkApplication *app,
                    UNUSED GFile **files,
                    UNUSED gint n_files,
                    UNUSED const gchar *hint,
                    UNUSED gpointer user_data)
{
	g_signal_emit_by_name(app, "activate");
}

static gboolean on_key_press(UNUSED GtkEventControllerKey *controller, guint keyval, UNUSED guint keycode, UNUSED GdkModifierType state, gpointer user_data) 
{
	switch (keyval)
	{
		case GDK_KEY_w:
			App.Inputs.up = true;
			break;
		case GDK_KEY_s:
			App.Inputs.down = true;
			break;
		case GDK_KEY_Escape:
			close_app();
		case GDK_KEY_e:
			{
				double x, y;

				GdkSurface *surface = gtk_native_get_surface(gtk_widget_get_native(user_data));
				GdkSeat *seat = gdk_display_get_default_seat(gdk_display_get_default());
				GdkDevice *pointer = gdk_seat_get_pointer(seat);

				gdk_surface_get_device_position(surface, pointer, &x, &y, NULL);

				set_ball(x, y, 0);
				printf("%f %f\n", x, y);
				break;
			}
		case GDK_KEY_q:
			set_ball(0.5, 0.5, 1);
			break;
	}
	return TRUE;
}

static gboolean on_key_release(UNUSED GtkEventControllerKey *controller, guint keyval, UNUSED guint keycode, UNUSED GdkModifierType state, UNUSED gpointer user_data)
{
	switch (keyval)
	{
		case GDK_KEY_w:
			App.Inputs.up = false;
			break;
		case GDK_KEY_s:
			App.Inputs.down = false;
			break;
	}
    return TRUE;
}

static void on_draw(UNUSED GtkDrawingArea *area, cairo_t *cr, int width, int height, UNUSED gpointer user_data)
{
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_paint(cr);

    int paddle_w = PX_W(PADDLE_W_N, width);
    int paddle_h = PX_H(PADDLE_H_N, height);

    int ball_r  = PX_H(BALL_RADIUS_N, height);
    double bx = App.GameState.bposx * width;
    double by = App.GameState.bposy * height;

    cairo_set_source_rgb(cr, 1, 1, 1);

    cairo_arc(cr, bx, by, ball_r, 0, 2 * M_PI);
    cairo_fill(cr);

    double left_x  = L_PAD_OFFSET_N * width;
    double right_x = width - R_PAD_OFFSET_N * width;

    double left_y_center  = App.GameState.lpos * height;
    double right_y_center = App.GameState.rpos * height;

    cairo_rectangle(
        cr,
        left_x,
        left_y_center - paddle_h / 2.0,
        paddle_w,
        paddle_h
    );

    cairo_rectangle(
        cr,
        right_x - paddle_w,
        right_y_center - paddle_h / 2.0,
        paddle_w,
        paddle_h
    );

    cairo_fill(cr);
}


static gboolean on_tick(GtkWidget *widget, UNUSED GdkFrameClock *frame_clock, UNUSED gpointer user_data) 
{
	get_game_data();
	if (App.Inputs.down)
		post_input(1);
	else if (App.Inputs.up)
		post_input(2);
	else
		post_input(0);
    gtk_widget_queue_draw(widget);
    return G_SOURCE_CONTINUE;
}

static void run_gtk(UNUSED GtkApplication *app, UNUSED gpointer user_data)
{
	GtkWidget *window = gtk_application_window_new(App.gtk);
	gtk_window_set_title(GTK_WINDOW(window), "PONG");
	gtk_window_set_default_size(GTK_WINDOW(window), 1920, 1080);
	gtk_window_present(GTK_WINDOW(window));

	GtkWidget *area = gtk_drawing_area_new();
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(area), on_draw, NULL, NULL);
	gtk_window_set_child(GTK_WINDOW(window), area);
	gtk_widget_add_tick_callback(area, on_tick, NULL, NULL);	

	GtkEventController *controller = gtk_event_controller_key_new();
	g_signal_connect(controller, "key-pressed", G_CALLBACK(on_key_press), area);
	g_signal_connect(controller, "key-released", G_CALLBACK(on_key_release), NULL);
	gtk_widget_add_controller(window, controller);
}

void init_gtk(int argc, char **argv)
{
	g_signal_connect(App.gtk, "open", G_CALLBACK(app_open), NULL);
	g_signal_connect(App.gtk, "activate", G_CALLBACK(run_gtk), NULL);
	g_application_run(G_APPLICATION(App.gtk), argc, argv);
}
