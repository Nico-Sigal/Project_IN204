// p2psocket.h
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

// create new type to safe some typing later
typedef std::list<wxSocketBase *> SocketList;
typedef std::vector<wxSocketClient*> SocketListClient;

// defining the flags for trasmetting specified data 
#define CHAT_FLAG 0xBB       // flag for chat message transfer

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
void OnActivate(wxCommandEvent & event);
void OnServerEvent(wxSocketEvent& event);
void OnServerSocketEvent(wxSocketEvent& event);
void OnClientSocketEvent(wxSocketEvent& event);
void ServerChat(wxSocketBase *sock);
void wxMySetupSocketEvents(wxSocketServer* pServerSocket, wxEvtHandler* pHandler);
void wxMySetupClient (wxSocketClient *pClientSocket, wxEvtHandler * pHandler);
void connect(wxString address);
void chat(wxString text);
void disconnect(wxString address);
void OnExit(wxCommandEvent & event);
void comGrille(pieces *matrice);
wxSocketServer *m_server;       //server socket;
SocketList m_socket_list;
SocketListClient m_socket_list_clients;
int lastclient = -1;
wxTextCtrl * logtext;
wxTextCtrl * commandtext;
wxListBox * otherservers, * otherclients;
wxArrayString servers_connected, clients_connected;
typedef std::vector<Joueur> Joueurs;
Joueurs listeJoueurs;
};

