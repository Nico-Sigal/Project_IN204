// p2psocket.cpp

#include "p2psocket.h"

mysocket :: mysocket (const wxString & title)
       : wxFrame(NULL, wxID_ANY, 
         title, 
         wxDefaultPosition, wxSize(760, 600), 
         wxDEFAULT_FRAME_STYLE)
{

wxStaticText * StaticTextServer = new wxStaticText (this, wxID_STATIC,
wxT ( "other &servers"),
wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);

otherservers = new wxListBox(this, wxID_ANY,
wxDefaultPosition, wxSize(180, 100), servers_connected, wxLB_SINGLE);

wxStaticText * StaticTextClient = new wxStaticText (this, wxID_STATIC,
wxT ( "other c&lients"),
wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);

otherclients = new wxListBox(this, wxID_ANY,
wxDefaultPosition, wxSize(180, 100), clients_connected, wxLB_SINGLE);

logtext = new wxTextCtrl (this, ID_LOG_TEXT, 
wxT(""), 
wxDefaultPosition, wxSize(700, 500),
wxTE_MULTILINE);

logtext->AppendText("Welcome to the program sample for p2psocket\nstart this program on all PCs connect to the network\n");
logtext->AppendText("\ntype on command line these commands:\n");
logtext->AppendText("\ncommand: connect <IP address> and press enter to connect \n(example: connect 192.168.1.88 end press enter)\n");
logtext->AppendText("\ncommand: chat <IP adress>: <message to send> and press enter to send message \n(example: chat 192.168.1.88: hello),press enter to send message\n");
logtext->AppendText("\ncommand: disconnect <IP address> and press enter to disconnect \n(example: disconnect 192.168.1.88 and press enter)\n");

wxStaticText * StaticTextCommand = new wxStaticText (this, wxID_STATIC,
wxT ( "&Command:"),
wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);

commandtext = new wxTextCtrl (this, ID_CMD_TEXT, 
wxT(""), 
wxDefaultPosition, wxSize(700, 20), 
wxTE_PROCESS_ENTER);

  Connect(ID_CMD_TEXT, wxEVT_COMMAND_TEXT_ENTER, 
      wxCommandEventHandler(mysocket :: OnActivate));

  wxButton * button_exit = new wxButton(this, wxID_EXIT, _("E&xit"));
  Connect(wxID_EXIT, wxEVT_COMMAND_BUTTON_CLICKED, 
      wxCommandEventHandler(mysocket :: OnExit));

wxSizer* sizertext = new wxBoxSizer(wxVERTICAL);
    wxSizer* sizerserverclient = new wxBoxSizer(wxHORIZONTAL);
        wxSizer* sizerserver = new wxBoxSizer(wxVERTICAL);
        sizerserver->Add(StaticTextServer, 0, wxALIGN_LEFT);
        sizerserver->Add(otherservers, 1, wxEXPAND);
        wxSizer* sizerclient = new wxBoxSizer(wxVERTICAL);
        sizerclient->Add(StaticTextClient, 0, wxALIGN_LEFT);
        sizerclient->Add(otherclients, 1, wxEXPAND);
sizerserverclient->Add(sizerserver, 1, wxEXPAND);
sizerserverclient->Add(sizerclient, 1, wxEXPAND);
sizertext->Add(sizerserverclient, 1, wxEXPAND);
sizertext->Add(logtext, 1, wxEXPAND);
    wxSizer* sizercommand = new wxBoxSizer(wxHORIZONTAL);
    sizercommand->Add(StaticTextCommand, 0, wxALIGN_LEFT);
    sizercommand->Add(commandtext, 1, wxEXPAND);
sizertext->Add(sizercommand, 1, wxEXPAND);
sizertext->Add(button_exit, 0, wxALIGN_CENTER);
SetSizer(sizertext);
sizertext->Fit(this);
sizertext->SetSizeHints(this);

// Create the address - defaults to localhost:0 initially
    wxIPV4address addr;
    addr.Service(3000);          //set listen port to 3000
    // Create the socket
  m_server = new wxSocketServer(addr);

  // We use Ok() here to see if the server is really listening
  if (! m_server->Ok())
  {
    logtext->AppendText(wxT("-= Could not listen at the specified port !"));
    return;
  }
  else
  {
    logtext->AppendText (wxT("-= Server listening on port 3000"));
  }

wxMySetupSocketEvents(m_server, this);
}

void mysocket :: OnActivate(wxCommandEvent & event)
{
wxString m_option = commandtext->GetValue();
commandtext->SetValue("");
if (m_option.Mid(0, 7) == "connect")
connect(m_option.Mid(8, wxStrlen(m_option)));
if (m_option.Mid(0, 4) == "chat")
chat(m_option.Mid(5, wxStrlen(m_option)));
if (m_option.Mid(0, 10) == "disconnect")
disconnect(m_option.Mid(11, wxStrlen(m_option)));
}

void mysocket :: OnExit(wxCommandEvent & event)
{
    Close(true);
}

mysocket :: ~mysocket()
{
delete m_server;                //destroy server socket
}

void mysocket :: wxMySetupSocketEvents(wxSocketServer* pServerSocket, wxEvtHandler* pHandler) 
{ 
// 
// Make sure there's a valid event handler 
// 
wxASSERT_MSG(pHandler != NULL || wxTheApp != NULL, wxT("NO EVENT HANDLER!")); 
// 
// We can use wxTheApp (aka &wxGetApp()) for events 
// if the handler is null 
// 
if (pHandler == NULL) 
pHandler = wxTheApp; 
// 
// Sets the event handler to recieve events 
// 
pServerSocket->SetEventHandler(*pHandler, SERVER_ID); 
// 
// Specify and connect the kind of events we want to recieve 
// Here we send server events to wxMyServer::OnServerEvent 
// and client events to wxMyServer::OnSocketEvent 
// 
pHandler->Connect(SERVER_ID, wxEVT_SOCKET, 
(wxObjectEventFunction) &mysocket :: OnServerEvent, NULL, this); 
pHandler->Connect(SERVERSOCKET_ID, wxEVT_SOCKET, 
(wxObjectEventFunction) &mysocket :: OnServerSocketEvent, NULL, this);
// 
// This can get involved but basically there 
// are several kinds of events we could recieve from 
// the server socket.... normally we just want connection 
// events 
// 
pServerSocket->SetNotify(wxSOCKET_CONNECTION_FLAG);
// 
// Tell the server we're ready to get events 
// (Other classes don't require this, this is kind 
// of a wxSockets-specific thing) 
// 
pServerSocket->Notify(true); 
}

void mysocket :: wxMySetupClient (wxSocketClient *pClientSocket, wxEvtHandler * pHandler)
{
// 
// Make sure there's a valid event handler 
// 
wxASSERT_MSG(pHandler != NULL || wxTheApp != NULL, wxT("NO EVENT HANDLER!")); 
// 
// We can use wxTheApp (aka &wxGetApp()) for events 
// if the handler is null 
// 
if (pHandler == NULL) 
pHandler = wxTheApp; 
// 
// Sets the event handler to recieve events 
// 
pClientSocket->SetEventHandler(*pHandler, CLIENTSOCKET_ID); 
// 
// Specify and connect the kind of events we want to recieve 
// from the client
// 
pHandler->Connect(CLIENTSOCKET_ID, wxEVT_SOCKET, 
(wxObjectEventFunction) &mysocket :: OnClientSocketEvent, NULL, this);
// 
// This can get involved but basically there 
// are several kinds of events we could recieve from 
// the client socket.... in this case we check
// the connection, the input and the lost from the client
// 
pClientSocket->SetNotify(wxSOCKET_CONNECTION_FLAG| 
wxSOCKET_INPUT_FLAG |
wxSOCKET_LOST_FLAG);
// 
// Tell the client we're ready to get events 
// (Other classes don't require this, this is kind 
// of a wxSockets-specific thing) 
// 
pClientSocket->Notify(true); 
}

void mysocket :: OnServerEvent(wxSocketEvent& event) 
{ 
//
// We only registered for connection events... make 
// sure we only got a connection event 
//
wxASSERT(event.GetSocketEvent() == wxSOCKET_CONNECTION); 
// 
// Get our server socket 
// 
wxSocketServer* pServerSocket = (wxSocketServer*) event.GetSocket(); 
// 
// Call accept, but since we know we'll get a socket we'll 
// tell it not to block 
// 
wxSocketBase * pSocket = pServerSocket->Accept(false); 
// 
// If a wierd fluke happens and we get a null socket break out 
// 
if(!pSocket) 
{
logtext->AppendText("\n-=unable to connect to the client...\n");
return; 
}
// 
// Setup for the client's events 
// We already called Connect for the server, so we don't 
// need to do it here 
// 
wxIPV4address addre;
pSocket->GetPeer(addre);                         //get peer adress
logtext->AppendText("\n-=connected to the client" + addre.IPAddress());
clients_connected.Add(addre.IPAddress());
otherclients->Set(clients_connected);
pSocket->SetEventHandler(*((wxEvtHandler*)this), SERVERSOCKET_ID); 
// 
// Recieve input events, which means the socket is ready 
// for reading writing etc.. 
// Lost event means the person disconnected from our server 
// and we need to destroy the socket 
// 
pSocket->SetNotify(wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG); 
// 
// We're ready to recieve client socket events 
// 
pSocket->Notify(true); 

// add a client to the list socket
m_socket_list.push_back(pSocket);
}

void mysocket :: OnServerSocketEvent(wxSocketEvent& event)
{
wxIPV4address address_lost;
wxSocketBase *sock = event.GetSocket();

// Now we process the event
switch(event.GetSocketEvent())
{
    case wxSOCKET_INPUT:                        //if socket input flag (socket sends data)
        // read the flag from the client to control the data type
        unsigned char c;
        sock->Read(&c, 1);    
        switch (c)
        {
            case CHAT_FLAG:                             //socket found a number
            ServerChat(sock);
            break;
      } // end switch control the data type
        break;
    case wxSOCKET_LOST:                         //in case when socket is lost
        sock->GetPeer(address_lost);
        logtext->AppendText(wxT("\n-= Deleted client "+address_lost.IPAddress()+" - connection lost"));
        clients_connected.Remove(address_lost.IPAddress());
        otherclients->Set(clients_connected);
        // remove a client from the list socket
        m_socket_list.remove(sock);
        sock->Destroy();                          //destroy socket
        break;
    } // end switch event.GetSocketEvent()
}

void mysocket :: ServerChat(wxSocketBase *sock)
{
#define MAX_MSG_SIZE 10000

  wxString message;
  char *buf = new char[MAX_MSG_SIZE];

  wxIPV4address clientaddre;
  wxIPV4address addre;
  sock->GetPeer(addre);                         //get peer adress

  // Read the message from client
  sock->ReadMsg(buf, MAX_MSG_SIZE);
  message = "\nClient " + addre.IPAddress() + " says: " + wxString::FromUTF8(buf, sock->LastCount() );
  unsigned char c = CHAT_FLAG;

int i = 0;
// iterating the socket on the list
  for (SocketList::iterator it = m_socket_list.begin(); it != m_socket_list.end(); ++it) 
  {
  wxSocketBase *socket = *it;
  socket->GetPeer(clientaddre);                         //get peer adress
  // write the reply to the all connected clients
  socket->Write(&c, 1);
  wxScopedCharBuffer cb = message.ToUTF8();
  socket->WriteMsg(cb, cb.length());                  //send message
}
  delete[] buf;

#undef MAX_MSG_SIZE
}

void mysocket :: connect(wxString address)
{
  typedef std::vector<wxIPV4address> IPClients;
  IPClients list_ip_clients;
  wxIPV4address addr;
  wxIPV4address address_verifing;
  wxString message;

  wxString hostname = address;
  addr.Hostname(hostname);
  addr.Service(3000);    

  // check if already connect with the client
  for (int i = 0; i<= lastclient; i++)
  {
      m_socket_list_clients[i]->GetPeer(address_verifing);
      list_ip_clients.push_back(address_verifing);
      if (list_ip_clients[i].IPAddress() == addr.IPAddress())
      {
          message = wxT("\nyou are already connected to the client: " + addr.IPAddress() + "\n");
          logtext->AppendText(message);
          return;
}
      }

  // create a new SocketClient to connected to the specified host
  m_socket_list_clients.push_back(new wxSocketClient());
  lastclient++;

  message = wxT("\n-= Trying to connect "+hostname+": " + "3000 (10 sec)...\n");
  logtext->AppendText(message);

  m_socket_list_clients[lastclient]->Connect(addr, false);
  m_socket_list_clients[lastclient]->WaitOnConnect(10);

  wxMySetupClient(m_socket_list_clients[lastclient], NULL);

  if (m_socket_list_clients[lastclient]->IsConnected())           //check if connected
  {
    message = wxT("-= Succeeded! Connection established\n");
    logtext->AppendText(message);
    m_socket_list_clients[lastclient]->GetLocal(addr);        
    message = wxT("-= Your IP: "+addr.IPAddress() + "\n");
    logtext->AppendText(message);
    m_socket_list_clients[lastclient]->GetPeer(addr);
    servers_connected.Add(addr.IPAddress());
    otherservers->Set(servers_connected);
  }
  else
  {
    m_socket_list_clients[lastclient]->Close();
    m_socket_list_clients.erase(m_socket_list_clients.begin() + lastclient);
    lastclient--;
    message = wxT("-= Failed! Unable to connect\nCan't connect to the specified host");
    logtext->AppendText(message);
  }
}

void mysocket :: disconnect(wxString address)
{
typedef std::vector<wxIPV4address> IPClients;
IPClients list_ip_clients;
list_ip_clients.clear();
bool present = false;
int j = 0;
wxIPV4address address_deleting;
wxIPV4address address_verifing;
address_deleting.Hostname(address);
wxString message;
message = wxT("\ncould not disconnect, address not connected\n");

// check the client for disconnecting
for (int i = 0; i <= lastclient; i++)
{
  m_socket_list_clients[i]->GetPeer(address_verifing);
  list_ip_clients.push_back(address_verifing);
  if (list_ip_clients[i].IPAddress() == address_deleting.IPAddress())
  {
    m_socket_list_clients[i]->Close();
    m_socket_list_clients.erase(m_socket_list_clients.begin() + i);
    lastclient--;
    for (int x = i; x < lastclient; x++)
        m_socket_list_clients[x] = m_socket_list_clients[x + 1];
      message = wxT("\nDisconnected from Server: " + list_ip_clients[i].IPAddress() + "\n");
    present = true;
    j = i;
    break;
  }
}
logtext->AppendText(message);

if (present == true)
{
    servers_connected.Remove(list_ip_clients[j].IPAddress());
    otherservers->Set(servers_connected);
}
}

void mysocket::comGrille(pieces *matrice) {
	wxString str;
	for (int i = 0; i < Longueur*Largeur; i++) {
		str += int(matrice[i]);
		str += ",";
	}
	chat(str);
}


void mysocket :: chat(wxString text)
{
	typedef std::vector<wxIPV4address> IPClients;
	IPClients list_ip_clients;
	wxIPV4address address_verifing;
	wxIPV4address address_sendto;
		const char *msg1;
	wxSocketBase *socket_client_selected=nullptr;
	bool present = false;
	int i, j = 0, k = 0, done = 0;
	wxString address;
	// check the address to send message at the server
	while (done == 0)
	{
	  if (text.Mid(j, 1) == ":")
	  {
		address = text.Mid(0, j);
		k = j + 1;
		text = text.Mid(j + 1, wxStrlen(text) + k);
		break;
	  }
	  else
		j++;
	  if (j == wxStrlen(text))
		done = 1;
	}

	if (address == "")
	  return;

	address_sendto.Hostname(address);
	for (i = 0; i <= lastclient; i++)
	{
	  m_socket_list_clients[i]->GetPeer(address_verifing);
	  list_ip_clients.push_back(address_verifing);
	  if (list_ip_clients[i].IPAddress() == address_sendto.IPAddress())
	  {
		socket_client_selected = m_socket_list_clients[i];
		present = true;
		break;
	  }
	}

	if (present == false)
	{
	  logtext->AppendText("\nyou aren't connected to the specified host\n");
	  return;
	}
	// Tell the server which test we are running
	unsigned char c = CHAT_FLAG;
	socket_client_selected->Write(&c, 1);

	// Here we use ReadMsg and WriteMsg to send messages with
	// a header with size information. Also, the reception is
	// event triggered, so we test input events as well.
	//
	// We need to set no flags here (ReadMsg and WriteMsg are
	// not affected by flags)

	wxScopedCharBuffer cb = text.ToUTF8();
	socket_client_selected->WriteMsg(cb, cb.length());                  //send message

	msg1 = socket_client_selected->Error() ? "\nMessage send - failed!" : "\nMessage send - done";
	logtext->AppendText(msg1);
}

pieces mysocket::messageEnMatrice(wxString message) {
		for
	}

	void mysocket :: OnClientSocketEvent(wxSocketEvent& event)
	{
	#define MAX_MSG_SIZE 10000
	char *buf = new char[MAX_MSG_SIZE];
	typedef std::vector<wxIPV4address> IPClients;
	IPClients list_ip_clients;
	list_ip_clients.clear();
	bool present = false;
	wxIPV4address address_deleting;
	wxIPV4address address_verifing;
	wxIPV4address addre;
	wxString message;

	wxSocketBase *sock = event.GetSocket();

	switch(event.GetSocketEvent())
	{
		case wxSOCKET_LOST : 
			sock->GetPeer(address_deleting);
			// check the client for disconnecting
			for (int i = 0; i <= lastclient; i++)
			{
			  m_socket_list_clients[i]->GetPeer(address_verifing);
			  list_ip_clients.push_back(address_verifing);
			  if (list_ip_clients[i].IPAddress() == address_deleting.IPAddress())
			  {
				  m_socket_list_clients.erase(m_socket_list_clients.begin() + i);
				  lastclient--;
				  for (int x = i; x < lastclient; x++)
					  m_socket_list_clients[x] = m_socket_list_clients[x + 1];
				  message = wxT("\nDisconnected from Server: " + list_ip_clients[i].IPAddress() + "\n");
				  present = true;
				  break;
			  }
			}
			if (present == true)
			{
				servers_connected.Remove(address_deleting.IPAddress());
				otherservers->Set(servers_connected);
				logtext->AppendText(message);
			}
			break;
		case wxSOCKET_INPUT: 
			if (!EnJeu) {
				// read the flag from the client to control the data type
				unsigned char c;
				sock->Read(&c, 1);
				switch (c)
				{
				case CHAT_FLAG:          //socket found a number
					sock->GetPeer(addre);                         //get peer adress
					sock->ReadMsg(buf, MAX_MSG_SIZE);
					message = wxString::FromUTF8(buf, sock->LastCount());
					logtext->AppendText(message);
					//display message  
					break;
				} // end switch control the data type
			}
			else {
				// read the flag from the client to control the data type
				unsigned char c;
				sock->Read(&c, 1);
				switch (c)
				{
				case CHAT_FLAG:          //socket found a number
					sock->GetPeer(addre);                         //get peer adress
					sock->ReadMsg(buf, MAX_MSG_SIZE);
					message = wxString::FromUTF8(buf, sock->LastCount());
					break;
				}
			}
	   break; 
	} // end switch event.GetSocketEvent()
	#undef MAX_MSG_SIZE
}

