
struct rte_flow *
create_flow(uint16_t nif, uint16_t qid, uint16_t portNo, 
            struct rte_flow_error *err)
{
	struct rte_flow_attr attr;
	struct rte_flow_item pattern[5];
	struct rte_flow_action action[5];
	struct rte_flow_action_queue queue = { .index = qid };
	struct rte_flow_item_eth eth_spec;
	struct rte_flow_item_eth eth_mask;
	//struct rte_flow_item_vlan vlan_spec;
	//struct rte_flow_item_vlan vlan_mask;
	struct rte_flow_item_ipv4 ip_spec;
	struct rte_flow_item_ipv4 ip_mask;
	struct rte_flow_item_tcp tcp_spec;
	struct rte_flow_item_tcp tcp_mask;
	//struct sockaddr_in *addr;
	struct rte_flow *flow;
	int rc;

	//addr = (struct sockaddr_in *)arg_ip_port;
	flow = NULL;
	memset(pattern, 0, sizeof(pattern));
	memset(action, 0, sizeof(action));

	/*
	 * set the rule attribute.
	 * in this case only ingress packets will be checked.
	 */
	memset(&attr, 0, sizeof(struct rte_flow_attr));
	attr.ingress = 1;

	/*
	 * create the action sequence.
	 * one action only, move packet to queue
	 */
	action[0].type = RTE_FLOW_ACTION_TYPE_QUEUE;
	action[0].conf = &queue;
	action[1].type = RTE_FLOW_ACTION_TYPE_END;

	/*
	 * set the first level of the pattern (eth).
	 * since in this case, we get to L4, we set this level
	 * to allow all.
	 */
	memset(&eth_spec, 0, sizeof(struct rte_flow_item_eth));
	memset(&eth_mask, 0, sizeof(struct rte_flow_item_eth));
	eth_spec.type = 0;
	eth_mask.type = 0;
	pattern[0].type = RTE_FLOW_ITEM_TYPE_ETH;
	pattern[0].spec = &eth_spec;
	pattern[0].mask = &eth_mask;
	
	/*
	 * setting the second level of the pattern (vlan).
	 * since in this case we want to get to all the
	 * way to L4, we also set this level to allow all.
	 */
	/*memset(&vlan_spec, 0, sizeof(struct rte_flow_item_vlan));
	memset(&vlan_mask, 0, sizeof(struct rte_flow_item_vlan));
	pattern[1].type = RTE_FLOW_ITEM_TYPE_VLAN;
	pattern[1].spec = &vlan_spec;
	pattern[1].mask = &vlan_mask;
    */
	/*
	 * setting the third level of the pattern (ip).
	 * in this case this is the level we care about
	 * so we set it according to the parameters.
	 */
	memset(&ip_spec, 0, sizeof(struct rte_flow_item_ipv4));
	memset(&ip_mask, 0, sizeof(struct rte_flow_item_ipv4));
	ip_spec.hdr.dst_addr = htonl(0xC0A8E864);
	ip_mask.hdr.dst_addr = 0xffffffff;
	ip_spec.hdr.src_addr = 0;
	ip_mask.hdr.src_addr = 0x0;
	pattern[1].type = RTE_FLOW_ITEM_TYPE_IPV4;
	pattern[1].spec = &ip_spec;
	pattern[1].mask = &ip_mask;

	/*
	 * setting the fourth level of the pattern (tcp).
	 * in this example this is the level we *really* care about
	 * so we set it according to the parameters.
	 */
	memset(&tcp_spec, 0, sizeof(struct rte_flow_item_tcp));
	memset(&tcp_mask, 0, sizeof(struct rte_flow_item_tcp));
	tcp_spec.hdr.dst_port = htons(portNo);
	tcp_mask.hdr.dst_port = 0xffff;
	tcp_spec.hdr.src_port = 0;
	tcp_mask.hdr.src_port = 0x0;
	pattern[2].type = RTE_FLOW_ITEM_TYPE_TCP;
	pattern[2].spec = &tcp_spec;
	pattern[2].mask = &tcp_mask;

	/* the final level must be always type end */
	pattern[3].type = RTE_FLOW_ITEM_TYPE_END;
    
    rc = rte_flow_validate(nif, &attr, pattern, action, err);
    if (!rc)
        flow = rte_flow_create(nif, &attr, pattern, action, err);
    else
      printf("Error creating flow.");
    /*
    if (qid == 0) {
        rc = rte_flow_validate(nif, &attr, pattern, action, err);
        if (!rc)
            flow = rte_flow_create(nif, &attr, pattern, action, err);
        
        qid = 1;
        int portNo = 9002;
        for (; qid < CONFIG.num_cores; qid++, portNo++) {
            queue.index = qid;
            action[0].conf = &queue; 
            tcp_spec.hdr.dst_port = htons(portNo);
            pattern[2].spec = &tcp_spec;

            rc = rte_flow_validate(nif, &attr, pattern, action, err);
            
            if (!rc)
                flow = rte_flow_create(nif, &attr, pattern, action, err);
       }
    } else {
       flow = (struct rte_flow *) &dpdk_module_func;
    }
    */
    return flow;
}
