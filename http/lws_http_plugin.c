#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include "lws_log.h"
#include "lws_http.h"
#include "lws_http_plugin.h"
#include "lws_util.h"

#define REGISTED 1
#define UNREGISTED 0


static int lws_check_register(void *p, char *data)
{
	struct http_message *hm = p;
    char user[20] = {0};
	char password[20] = {0};
	int flag = UNREGISTED;

	sprintf(user,"%.*s",hm->user.len, hm->user.p);
	sprintf(password,"%.*s",hm->password.len, hm->password.p);
	lws_log(4,"user：%s\n",user);
	lws_log(4,"password：%s\n",password);
	
	if((strcmp(user,"123456") && strcmp(password,"123456")) != 0) {

		flag = UNREGISTED;
		sprintf(data,
		"<html><body>"
		"<a href=\"/\"> echo check here for login </a>"
		"</body></html>");
	}
	else {
		flag = REGISTED;
	}
	return flag;
}

int lws_register_handler(lws_http_conn_t *c, int ev, void *p)
{
	struct http_message *hm = p;
	char data[1024] = {0};

	if (hm && ev == LWS_EV_HTTP_REQUEST) {
		if (c->send == NULL) {
			return HTTP_INTERNAL_SERVER_ERROR;
		}

		sprintf(data, "%s",
				  "<html><body><form method =\"POST\" action=\"http://192.168.1.141:8000/default\">"
				  "<table border=\"5\" align=\"ceter\">"
				  "<tr><td colspan=\"2\" align=\"center\" height=\"40\">LOG IN </td></tr>"
					"<tr><td align=\"right\">USER:</td>"
					"<td><input type=\"text\" name=\"userName\" value=\"\"/></td></tr>"
					"<tr><td align=\"right\">PWD:</td>"
					"<td><input type=\"password\" name=\"userPwd\" value=\"\"/></td></tr>"
					"</table>"
					"<input type=\"submit\" name=\"button\" id=\"button\" value=\"submit\"/></br> </body></html>");

		lws_http_respond(c, 200, c->close_flag, LWS_HTTP_HTML_TYPE, data, strlen(data));
	}
	else {
		return HTTP_BAD_REQUEST;
	}
	
	return HTTP_OK;
}

int lws_default_handler(lws_http_conn_t *c, int ev, void *p)
{
    struct http_message *hm = p;
	char data[1024] = {0};
	int code_flag = UNREGISTED;					
	char data_for_register[1024] = {0};

    if (hm && ev == LWS_EV_HTTP_REQUEST) {
        if (c->send == NULL) {
            return HTTP_INTERNAL_SERVER_ERROR;
        }
		
	/*veidict if have the right username and password*/
		code_flag = lws_check_register(hm, data_for_register);
		sprintf(data, "%s",
						"<html><body><h>Enjoy your webserver!</h>"
						"<hr style=\"width:160px;color:#00ffff;position:absolute;left:10px;\"><br/><br/>"
						"<ul style=\"list-style-type:circle\">"
						"<li><a href=\"/default\"> echo root diretory </a></li>"
						"<li><a href=\"/hello\"> echo hello message </a></li>"
						"<li><a href=\"/version\"> echo lws version </a></li>"
						"<li><a href=\"/download\"> downlad file </a></li>"
						"</ul>"
						"</body></html>");
		if(code_flag){
			lws_http_respond(c, 200, c->close_flag, LWS_HTTP_HTML_TYPE, data, strlen(data));
			}
		else{
			lws_http_respond(c, 200, c->close_flag, LWS_HTTP_HTML_TYPE, data_for_register, strlen(data));
		}
    } 
	else {
        return HTTP_BAD_REQUEST;
    }

    return HTTP_OK;
}

/*
int lws_default_handler(lws_http_conn_t *c, int ev, void *p)
{
	struct http_message *hm = p;
	char data[1024] = {0};
	//char new_data[1024] = {0};
	//char user[20] = {0};
	//char password[20] = {0};

	if (hm && ev == LWS_EV_HTTP_REQUEST) {
		if (c->send == NULL) {
			return HTTP_INTERNAL_SERVER_ERROR;
		}

		sprintf(data, "%s",
				  "<html><body><form method =\"POST\" action=\"http://192.168.1.141:8000/download\">"
				  "<table border=\"5\" align=\"ceter\">"
				  "<tr><td colspan=\"2\" align=\"center\" height=\"40\">LOG IN </td></tr>"
					"<tr><td align=\"right\">USER:</td>"
					"<td><input type=\"text\" name=\"userName\" value=\"\"/></td></tr>"
					"<tr><td align=\"right\">PWD:</td>"
					"<td><input type=\"password\" name=\"userPwd\" value=\"\"/></td></tr>"
					"</table>"
					"<input type=\"submit\" name=\"button\" id=\"button\" value=\"submit\"/></br> </body></html>");

		lws_http_respond(c, 401, c->close_flag, LWS_HTTP_HTML_TYPE, data, strlen(data));
	} else {
		return HTTP_BAD_REQUEST;
	}

	return HTTP_OK;
}
*/

int lws_hello_handler(lws_http_conn_t *c, int ev, void *p)
{
    struct http_message *hm = p;
    char data[1024] = {0};

    if (hm && ev == LWS_EV_HTTP_REQUEST) {
        if (c->send == NULL) {
            return HTTP_INTERNAL_SERVER_ERROR;
        }

        sprintf(data, "%s",
                  "<html><body><h>Hello LWS!</h><br/><br/>"
                  "</body></html>");

        lws_http_respond(c, 200, c->close_flag, LWS_HTTP_HTML_TYPE, data, strlen(data));
    } else {
        return HTTP_BAD_REQUEST;
    }

    return HTTP_OK;
}

int lws_version_handler(lws_http_conn_t *c, int ev, void *p)
{
    struct http_message *hm = p;
    char data[1024] = {0};

    if (hm && ev == LWS_EV_HTTP_REQUEST) {
        if (c->send == NULL) {
            return HTTP_INTERNAL_SERVER_ERROR;
        }

        sprintf(data, "<html><body><h>LWS - version[%s]</h><br/><br/>"
                  "</body></html>", LWS_HTTP_VERSION);

        lws_http_respond(c, 200, c->close_flag, LWS_HTTP_HTML_TYPE, data, strlen(data));
    } else {
        return HTTP_BAD_REQUEST;
    }

    return HTTP_OK;
}

int lws_show_handler(lws_http_conn_t *c, int ev, void *p)
{
    struct http_message *hm = p;
    char *data = NULL;
    char *filename = NULL;
    long filesize = 0;
    int rlen = 0;
    char uri[128] = {0};
    char path[128] = {0};

    if (hm && ev == LWS_EV_HTTP_REQUEST) {
        strncpy(uri, hm->uri.p, hm->uri.len);
        filename = lws_basename(uri);
        if (filename == NULL)
            return HTTP_INTERNAL_SERVER_ERROR;

        sprintf(path, "./load/%s", filename);
        lws_log(4, "path: %s\n", path);
        filesize = lws_ftell_file(path);
        if (filesize <= 0)
            return HTTP_INTERNAL_SERVER_ERROR;

        lws_log(4, "filesize: %d\n", filesize);
        data = malloc(filesize);
        rlen = lws_read_file(path, data, filesize);
        if (rlen != filesize) {
            lws_log(2, "rlen: %d\n", rlen);
            free(data);
            return HTTP_INTERNAL_SERVER_ERROR;
        }

        lws_http_respond(c, 200, c->close_flag, LWS_HTTP_JPEG_TYPE, data, rlen);
        free(data);
    } else {
        return HTTP_BAD_REQUEST;
    }

    return HTTP_OK;
}

int lws_binary_handler(lws_http_conn_t *c, int ev, void *p)
{
    struct http_message *hm = p;
    char *data = NULL;
    char *filename = NULL;
    long filesize = 0;
    int rlen = 0;
    char uri[128] = {0};
    char path[128] = {0};

    if (hm && ev == LWS_EV_HTTP_REQUEST) {
        strncpy(uri, hm->uri.p, hm->uri.len);
        filename = lws_basename(uri);
        if (filename == NULL)
            return HTTP_INTERNAL_SERVER_ERROR;

        sprintf(path, "./load/%s", filename);
        lws_log(4, "path: %s\n", path);
        filesize = lws_ftell_file(path);
        if (filesize <= 0)
            return HTTP_INTERNAL_SERVER_ERROR;

        lws_log(4, "filesize: %d\n", filesize);
        data = malloc(filesize);
        rlen = lws_read_file(path, data, filesize);
        if (rlen != filesize) {
            lws_log(2, "rlen: %d\n", rlen);
            free(data);
            return HTTP_INTERNAL_SERVER_ERROR;
        }

        lws_http_respond(c, 200, c->close_flag, LWS_HTTP_OCTET_STREAM, data, rlen);
        free(data);
    } else {
        return HTTP_BAD_REQUEST;
    }

    return HTTP_OK;
}

int lws_download_handler(lws_http_conn_t *c, int ev, void *p)
{
    struct http_message *hm = p;
    char uri[1024] = {0};
    char path[1024] = {0};
    char *filename;
    struct stat s_buf;
    char *data = NULL;
    long filesize = 0;
    int rlen = 0;
    DIR *dp = NULL;
    struct dirent *dir;


    if (hm == NULL || ev != LWS_EV_HTTP_REQUEST)
        return HTTP_BAD_REQUEST;

    if (hm->uri.len > 1024) {
        lws_log(2, "Error, request size: %d\n", hm->uri.len);
        return HTTP_BAD_REQUEST;
    }

	
	
    lws_log(4, "%.*s\n", hm->uri.len, hm->uri.p);
    strncpy(uri, hm->uri.p, hm->uri.len);
    filename = uri + strlen("/download");
    if (filename == NULL)
        sprintf(path, "./");
    else {
        sprintf(path, "./%s", filename);
    }

    if (access(path, F_OK) != 0) {
        lws_log(2, "path[%s] is not exist\n", path);
        return HTTP_NOT_FOUND;
    }

    stat(path, &s_buf);
    if (S_ISDIR(s_buf.st_mode)) {
        lws_log(4, "show dir: %s\n", path);

        data = malloc(4 * 1024);
        rlen += sprintf(data + rlen, "<html><head><title>%s</title></head><body>", path);
        rlen += sprintf(data + rlen, "<h1>Index of %s</h1>", path);
		rlen += sprintf(data + rlen, 
				 					"<ul style=\"list-style-type:circle\">"
                    				"<li><a href=\"/default\"> echo root diretory </a></li>");
        dp = opendir(path);
        while ((dir = readdir(dp)) != NULL) {
            if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
                continue;

            if (dir->d_type == DT_DIR) {
                rlen += sprintf(data + rlen, "<a href=\"%s/%s\">./%s</a></br>", uri, dir->d_name, dir->d_name);
            } else {
                rlen += sprintf(data + rlen, "<a href=\"%s/%s\">%s</a></br>", uri, dir->d_name, dir->d_name);
            }
        }
        closedir(dp);
        rlen += sprintf(data + rlen, "</body></html>");
        lws_log(4, "response: %.*s\n", rlen, data);
        lws_http_respond(c, 200, c->close_flag, LWS_HTTP_HTML_TYPE, data, rlen);
        free(data);
    } else if (S_ISREG(s_buf.st_mode)) {
        lws_log(4, "show file: %s\n", path);
        filesize = lws_ftell_file(path);
        if (filesize <= 0)
            return HTTP_INTERNAL_SERVER_ERROR;

        lws_log(4, "filesize: %d\n", filesize);
        data = malloc(filesize);
        rlen = lws_read_file(path, data, filesize);
        if (rlen != filesize) {
            lws_log(2, "rlen: %d\n", rlen);
            free(data);
            return HTTP_INTERNAL_SERVER_ERROR;
        }

        lws_http_respond(c, 200, c->close_flag, lws_http_contenttype(path), data, rlen);
        free(data);
    }

    return HTTP_OK;
}


