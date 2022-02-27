#include <gtk/gtk.h>
#include <stdio.h>
#include <json-c/json.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


GtkWidget *windows; // Variable pour la fenetre
GtkWidget *windows_settings; // Variable pour la fenetre
GtkWidget *windows_game;
GtkWidget *windows_result;

char *json_received;

struct curl_fetch_st {
    char *payload;
    size_t size;
};

/* callback for curl fetch */
size_t curl_callback (void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;                             /* calculate buffer size */
    struct curl_fetch_st *p = (struct curl_fetch_st *) userp;   /* cast pointer to fetch struct */

    p->payload = (char *) realloc(p->payload, p->size + realsize + 1);
    if (p->payload == NULL) {
        fprintf(stderr, "ERROR: Failed to expand buffer in curl_callback");
        free(p->payload);
        return -1;
    }

    /* copy contents to buffer */
    memcpy(&(p->payload[p->size]), contents, realsize);
    p->size += realsize;
    p->payload[p->size] = 0;
    return realsize;
}

/* fetch and return url body via curl */
CURLcode curl_fetch_url(CURL *ch, const char *url, struct curl_fetch_st *fetch) {
    CURLcode rcode;                   /* curl result code */

    fetch->payload = (char *) calloc(1, sizeof(fetch->payload));
    if (fetch->payload == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate payload in curl_fetch_url");
        return CURLE_FAILED_INIT;
    }

    fetch->size = 0;
    curl_easy_setopt(ch, CURLOPT_URL, url);
    curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, curl_callback);
    curl_easy_setopt(ch, CURLOPT_WRITEDATA, (void *) fetch);
    curl_easy_setopt(ch, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(ch, CURLOPT_TIMEOUT, 5);
    curl_easy_setopt(ch, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(ch, CURLOPT_MAXREDIRS, 1);

    /* fetch the url */
    rcode = curl_easy_perform(ch);
    return rcode;
}

struct curl_fetch_st curl_fetch;                        /* curl fetch struct */

char *curl_receive_reply_json_to_buffer(char *url, char *userpwd) {
    CURL *ch;                                               /* curl handle */
    CURLcode rcode;                                         /* curl result code */
    struct curl_fetch_st *cf = &curl_fetch;                 /* pointer to fetch struct */
    struct curl_slist *headers = NULL;                      /* http headers to send with request */

    /*TODO: when to free above malloced url*/

    /* init curl handle */
    if ((ch = curl_easy_init()) == NULL) {
        /* log error */
        fprintf(stderr, "ERROR: Failed to create curl handle in fetch_session");
        /* return error */
        return NULL;
    }

    /* set content type */
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(ch, CURLOPT_HTTPGET, 1L);

    if (userpwd != NULL) {
        curl_easy_setopt(ch, CURLOPT_USERPWD, userpwd);
        curl_easy_setopt(ch, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(ch, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(ch, CURLOPT_SSL_VERIFYHOST, 0);
    }

    /* fetch page and capture return code */
    rcode = curl_fetch_url(ch, url, cf);

    /* cleanup curl handle */
    curl_easy_cleanup(ch);

    /* free headers */
    curl_slist_free_all(headers);
    /* check return code */
    if (rcode != CURLE_OK || cf->size < 1) {
        /* log error */
        fprintf(stderr, "ERROR: Failed to fetch url (%s) - curl said: %s",
                url, curl_easy_strerror(rcode));
        /* return error */
        return NULL; // TODO: how to handle "curl said: Timeout was reached"
    }

    /* check payload */
    if (cf->payload != NULL) {
        /* print result */
        return cf->payload;
    }

    /* error */
    fprintf(stderr, "ERROR: Failed to populate payload");
    /* free payload */
    free(cf->payload);
    /* return */
    return NULL;
}

char *curl_download_json_to_buffer(char *remote_url) {
    char *reply_json_received;
    int wait_count = 0;

    while(wait_count < 10) {
        reply_json_received = curl_receive_reply_json_to_buffer(remote_url, NULL);
        if (reply_json_received == NULL) {
            wait_count++;
            printf("failed for %d th time, retrying after 3 sec...\n", wait_count);
            sleep(5); //sleep for 5 sec and retry
            continue;
        } else
            break;
    }


    json_received = reply_json_received;

    return reply_json_received;
}

int main(int argc, char *argv[]) {
    // Curl
    curl_download_json_to_buffer("http://146.59.154.231/api_myges/index.php");

    // Parser JSON
    struct json_object *parsed_json;
    struct json_object *firstname;
    struct json_object *name;
    struct json_object *photo;
    struct json_object *course;
    struct json_object *grades;
    struct json_object *teacher_first_name;
    struct json_object *teacher_last_name;
    struct json_object *value;
    struct json_object *value1;
    struct json_object *value_in_1;


    parsed_json = json_tokener_parse(json_received);

    value = json_object_array_get_idx(parsed_json, 0);
    value1 = json_object_array_get_idx(parsed_json, 1);

    // printf("\nICI: %s\n", json_object_get_string(value_in_1));

    json_object_object_get_ex(value, "firstname", &firstname);
    json_object_object_get_ex(value, "name", &name);
    json_object_object_get_ex(value, "photo", &photo);


    printf("\nPrénom: %s\n", json_object_get_string(firstname));
    printf("Nom: %s\n", json_object_get_string(name));
    printf("Photo: %s\n", json_object_get_string(photo));

    for (int i=0; i<13; i++){
        value_in_1 = json_object_array_get_idx(value1, i);

        json_object_object_get_ex(value_in_1, "course", &course);
        json_object_object_get_ex(value_in_1, "grades", &grades);
        json_object_object_get_ex(value_in_1, "teacher_first_name", &teacher_first_name);
        json_object_object_get_ex(value_in_1, "teacher_last_name", &teacher_last_name);

        printf("\nMatiere: %s\n", json_object_get_string(course));
        printf("Notes: %s\n", json_object_get_string(grades));
        printf("Prenom du professeur: %s\n", json_object_get_string(teacher_first_name));
        printf("Nom du professeur: %s\n", json_object_get_string(teacher_last_name));
    }


    // Gtk/Glade
    GtkBuilder *builder; // Pour le build
    gtk_init(&argc, &argv); // Lancer gtk

    builder = gtk_builder_new(); // Crée le build
    gtk_builder_add_from_file(builder, "start.glade", NULL); // Charger le fichier

    windows = GTK_WIDGET(gtk_builder_get_object(builder, "start")); // Charger la fenetre

    // Nécessaire pour un code qui utilise gtk
    gtk_builder_connect_signals(builder, NULL);
    g_object_unref(builder);

    gtk_widget_show_all(windows); // Afficher notre fenetre
    gtk_main(); // Lunch

    return 0;
}
// Pour quitter l'app
void exit_app(){
    printf("Quiiter \n");
    gtk_main_quit(); // commande pour quitter
}

// Reaction du button
void button_settings(){
    GtkBuilder *builder;
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "settings.glade", NULL); // Charger le fichier

    windows_settings = GTK_WIDGET(gtk_builder_get_object(builder, "windows_settings"));
    gtk_window_set_default_size(GTK_WINDOW(windows_settings), 800, 400);
    g_signal_connect(windows_settings, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_builder_connect_signals(builder, NULL);
    gtk_widget_show_all(windows_settings);
}

void destroy_settings(){
    gtk_widget_destroy(windows_settings);
}

void button_game(){
    GtkBuilder *builder;
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "game.glade", NULL); // Charger le fichier

    windows_game = GTK_WIDGET(gtk_builder_get_object(builder, "windows_game"));
    gtk_window_set_default_size(GTK_WINDOW(windows_game), 1920, 1080);
    g_signal_connect(windows_game, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_builder_connect_signals(builder, NULL);
    gtk_widget_show_all(windows_game);
}

void button_result(){
    GtkBuilder *builder;
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "result.glade", NULL); // Charger le fichier

    windows_result = GTK_WIDGET(gtk_builder_get_object(builder, "windows_result"));
    gtk_window_set_default_size(GTK_WINDOW(windows_result), 1920, 1080);
    g_signal_connect(windows_result, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_builder_connect_signals(builder, NULL);
    gtk_widget_show_all(windows_result);
}

