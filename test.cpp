void mysocket::chat(wxString text)
{
	typedef std::vector<wxIPV4address> IPClients;
	IPClients list_ip_clients;
	wxIPV4address address_verifing;
	wxIPV4address address_sendto;
	const char *msg1;
	wxSocketBase *socket_client_selected = nullptr;
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
