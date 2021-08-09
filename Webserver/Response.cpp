#include "incl.hpp"
/*
Response::respond() {
    try {
		if ((strncmp(buf, "GET", 3)) == 0) {
			this->doGET(sender, &buf[5], server);
		}
		else if (strncmp(buf, "POST", 4) == 0) {
			this->doPOST(sender, &buf[6], server);
		}
        else if (strncmp(buf, "PUT", 3) == 0) {
			this->doPUT(sender, &buf[5], server);
		}
        else if (strncmp(buf, "DELETE", 6) == 0) {
			this->doDELETE(sender, &buf[8], server);
		}
        else
            this->noneMethod(sender, buf, server);
	}
	catch (Webserver::GETException& e) {
		std::cerr << e.what() << ": " << strerror(errno) << std::endl;
		this->sendFile(sender, this->NotFound_repl->c_str(), this->NotFound_repl->size(), "404_not_found.html");
		close(sender);
	}
	catch (std::exception& e) {
		std::cerr << "Undefined error: " << e.what() << std::endl;
	}
}*/