// p2psocket.h
#pragma once
#include <vector>
#include <list>
#include <algorithm>
#include <wx/wx.h>
#include "wx/socket.h"
#include "wx/url.h"
#include "wx/sstream.h"
#include "wx/thread.h"
#include "Puit.h"
#include "Joueur.h"
#include "JoueurEnnemi.h"
#include "Tetris.h"
#include "main.h"

// create new type to safe some typing later
typedef std::list<wxSocketBase *> SocketList;
typedef std::vector<wxSocketClient*> SocketListClient;

// defining the flags for trasmetting specified data 
#define CHAT_FLAG 0xBB       // flag for chat message transfer
#define PLAY_FLAG 0xBA



enum
{
SERVER_ID = 1,
SERVERSOCKET_ID,
CLIENTSOCKET_ID,
ID_LOG_TEXT,
ID_CMD_TEXT,
ID_THREAD
};

class mysocket : public wxFrame
{
public:
mysocket (const wxString & title);
~mysocket();
enum { Largeur = 10, Hauteur = 22 };
void OnActivate(wxCommandEvent & event);
void OnServerEvent(wxSocketEvent& event);
void OnServerSocketEvent(wxSocketEvent& event);
void OnClientSocketEvent(wxSocketEvent& event);
void ServerChat(wxSocketBase *sock);
void wxMySetupSocketEvents(wxSocketServer* pServerSocket, wxEvtHandler* pHandler);
void wxMySetupClient (wxSocketClient *pClientSocket, wxEvtHandler * pHandler);
void connect(wxString address);
void play();
void chat(wxString text);
void disconnect(wxString address);
void OnExit(wxCommandEvent & event);
void comGrille(pieces *matrice);
void messageEnMatrice(wxString message,pieces* nouvelleMatrice);
wxSocketServer *m_server;       //server socket;
SocketList m_socket_list;
SocketListClient m_socket_list_clients;
int lastclient = -1;
wxTextCtrl * logtext;
wxTextCtrl * commandtext;
wxListBox * otherservers, * otherclients;
wxSizer* sizertext,* sizerserver,* sizerclient,* sizercommand,* sizerserverclient;
wxArrayString servers_connected, clients_connected;
boolean EnJeu=false;
typedef std::vector<JoueurEnnemi> Joueurs;
Joueurs listeJoueurs;
unsigned int nbEnnemis;
Joueur* player1;
};

