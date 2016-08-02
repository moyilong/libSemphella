void Hert_beat_server(struct DATA_FORMAT in, struct DATA_FORMAT &ret, SOCKET &conn);
void Hert_beat_client_to(struct DATA_FORMAT &data);
void Hert_beat_client_ret(struct DATA_FORMAT &data);
void Data_Routing_Server(struct DATA_FORMAT in, struct DATA_FORMAT &ret, SOCKET &conn);
int dev_size();
int routing_size();
ROUTING_INFO GetRouting(int n);
DEVICE GetDevice(int n);