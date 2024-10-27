#include "uepch.h"
#include "PlatformUtils.h"
#include "Application.h"

#include <gtk/gtk.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

// https://docs.gtk.org/gtk3/class.FileChooserDialog.html
// https://stackoverflow.com/questions/64517924/how-do-you-create-a-gtk-window-handle-from-an-x11-window-handle


namespace Ugly {
   	float Time::GetTime()
	{
		return glfwGetTime();
	}


	std::string FileDialogs::OpenFile(const char* filter){
        int argc = 0;
        gtk_init(&argc, nullptr);

        GtkWidget *openFileDialog;
        gint res;
        GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
        openFileDialog = gtk_file_chooser_dialog_new(
            "Open File",
            nullptr,  // No parent window, as we're using stacking hints instead
            action,
            "Cancel", GTK_RESPONSE_CANCEL,
            "Open", GTK_RESPONSE_ACCEPT,
            nullptr
        );

        // Handle dialog showing
        gtk_widget_show(openFileDialog);

        res = gtk_dialog_run (GTK_DIALOG(openFileDialog));
        if (res == GTK_RESPONSE_ACCEPT) {
            char *filename;
            GtkFileChooser *chooser = GTK_FILE_CHOOSER(openFileDialog);
            filename = gtk_file_chooser_get_filename(chooser);
            std::cout << filename << std::endl;
            if (filename){
                gtk_widget_destroy(openFileDialog);
                while (g_main_context_iteration(nullptr, false));
                return filename;
                //g_free(filename);
            }
        }
        gtk_widget_destroy(openFileDialog);
        while (g_main_context_iteration(nullptr, false));
		return std::string();
	}


	std::string FileDialogs::SaveFile(const char* filter) {
        int argc = 0;
        gtk_init(&argc, nullptr);

        // Step 1: Get the X11 Window ID of the GLFW window
        GtkWidget *dialog;
        GtkFileChooser *chooser;
        GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
        gint res;
        
        dialog = gtk_file_chooser_dialog_new (
            "Save File",
            nullptr,
            action,
            ("_Cancel"),
            GTK_RESPONSE_CANCEL,
            ("_Save"),
            GTK_RESPONSE_ACCEPT,
            NULL
        );
        // Handle dialog showing
        gtk_widget_show(dialog);

        chooser = GTK_FILE_CHOOSER (dialog);
        
        gtk_file_chooser_set_do_overwrite_confirmation (chooser, TRUE);
        gtk_file_chooser_set_current_name (chooser, ("Untitled.ugly"));
        
        res = gtk_dialog_run (GTK_DIALOG (dialog));
        if (res == GTK_RESPONSE_ACCEPT)
          {
            char *filename;
        
            filename = gtk_file_chooser_get_filename(chooser);
            gtk_widget_destroy(dialog);
            while (g_main_context_iteration(nullptr, false));
            return filename;
            //g_free (filename);
          }
        
        gtk_widget_destroy(dialog);
        while (g_main_context_iteration(nullptr, false));

		return std::string();
	}
};
