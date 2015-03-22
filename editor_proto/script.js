            var instance;
            jsPlumb.ready(function() {

           instance = jsPlumb.getInstance({
                // default drag options
                DragOptions : { cursor: 'pointer', zIndex:2000 },
                // the overlays to decorate each connection with.  note that the label overlay uses a function to generate the label text; in this
                // case it returns the 'labelText' member that we set on each connection in the 'init' method below.
                ConnectionOverlays : [
                    [ "Arrow", { location:1 } ] //,
                    //[ "Label", { 
                    //    location:0.1,
                    //    id:"label",
                    //    cssClass:"aLabel"
                    //}
                ],
                Container:"editDiv"
            });

            // this is the paint style for the connecting lines..
            var connectorPaintStyle = {
                lineWidth:4,
                strokeStyle:"#61B7CF",
                joinstyle:"round",
                outlineColor:"white",
                outlineWidth:2
            },
            // .. and this is the hover style. 
            connectorHoverStyle = {
                lineWidth:4,
                strokeStyle:"#216477",
                outlineWidth:2,
                outlineColor:"white"
            },
            endpointHoverStyle = {
                fillStyle:"#216477",
                strokeStyle:"#216477"
            },
            // the definition of source endpoints (the small blue ones)
            sourceEndpoint = {
                endpoint:"Dot",
                paintStyle:{ 
                    strokeStyle:"#7AB02C",
                    fillStyle:"transparent",
                    radius:7,
                    lineWidth:3 
                },				
                isSource:true,
                connector:[ "Flowchart", { stub:[40, 60], gap:10, cornerRadius:5, alwaysRespectStubs:true } ],								                
                connectorStyle:connectorPaintStyle,
                hoverPaintStyle:endpointHoverStyle,
                connectorHoverStyle:connectorHoverStyle,
                dragOptions:{},
            },		
            // the definition of target endpoints (will appear when the user drags a connection) 
            targetEndpoint = {
                endpoint:"Dot",					
                paintStyle:{ fillStyle:"#7AB02C",radius:11 },
                hoverPaintStyle:endpointHoverStyle,
                maxConnections:-1,
                dropOptions:{ hoverClass:"hover", activeClass:"active" },
                isTarget:true,			
            },			
            init = function(connection) {			
                connection.getOverlay("label").setLabel(connection.sourceId.substring(15) + "-" + connection.targetId.substring(15));
                connection.bind("editCompleted", function(o) {
                    if (typeof console != "undefined")
                        console.log("connection edited. path is now ", o.path);
                });
            };			

            var _addEndpoints = function(toId, sourceAnchors, targetAnchors) {
                    for (var i = 0; i < sourceAnchors.length; i++) {
                        var sourceUUID = toId + sourceAnchors[i];
                        instance.addEndpoint(toId, sourceEndpoint, { anchor:sourceAnchors[i], uuid:sourceUUID });						
                    }
                    for (var j = 0; j < targetAnchors.length; j++) {
                        var targetUUID = toId + targetAnchors[j];
                        instance.addEndpoint(toId, targetEndpoint, { anchor:targetAnchors[j], uuid:targetUUID });						
                    }
                };

            // suspend drawing and initialise.
            instance.doWhileSuspended(function() {
                // listen for new connections; initialise them the same way we initialise the connections at startup.
                instance.bind("connection", function(connInfo, originalEvent) { 
                    init(connInfo.connection);
                });			

                // listen for clicks on connections, and offer to delete connections on click.
                instance.bind("click", function(conn, originalEvent) {
                    jsPlumb.detach(conn); 
                });	

                instance.bind("connectionDrag", function(connection) {
                    console.log("connection " + connection.id + " is being dragged. suspendedElement is ", connection.suspendedElement, " of type ", connection.suspendedElementType);
                });		

                instance.bind("connectionDragStop", function(connection) {
                    console.log("connection " + connection.id + " was dragged");
                });

                instance.bind("connectionMoved", function(params) {
                    console.log("connection " + params.connection.id + " was moved");
                });
            });

            jsPlumb.fire("jsPlumbDemoLoaded", instance);

        });
            
            var counter = 0;
            var init = true;
            
            // Parse the XML-File to get possible blocks
            function parseXml() {
                if(!init) return;
                xmlhttp=new XMLHttpRequest();
                xmlhttp.open("GET","blocks.xml",false);
                xmlhttp.send();
                xmlDoc=xmlhttp.responseXML;
                
                // Get Blocks from XML
                var blocks = xmlDoc.getElementsByTagName("block");
                for(var i=0; i < blocks.length; i++)
                {
                    // collect information and create blockfactory
                    var current = blocks[i];
                    var name = current.getAttribute("name");
                    var inputs = current.getElementsByTagName("input");
                    var outputs = current.getElementsByTagName("output");
                    addBlockFactory(name, inputs, outputs, false);
                }
                var blocks = xmlDoc.getElementsByTagName("stdBlock");
                for(var i=0; i < blocks.length; i++)
                {
                    // collect information and create blockfactory
                    var current = blocks[i];
                    var name = current.getAttribute("name");
                    var inputs = current.getElementsByTagName("input");
                    var outputs = current.getElementsByTagName("output");
                    addBlockFactory(name, inputs, outputs, true);
                }
            }
            
            // Create the BlockFactory
            function addBlockFactory(name, inputList, outputList, std)
            {
                // create div..
                var returnNode = document.createElement("div");
                returnNode.className = "blockFactory";
                returnNode.id = name;
                returnNode.setAttribute("draggable", "true");
                returnNode.setAttribute("ondragstart", "drag(event)");

                // create header..
                var header = document.createElement("header");
                header.innerHTML = name;
                header.id = returnNode.id + "_header";
                returnNode.appendChild(header);
                
                // create table for input / output
                var table = document.createElement("table");
                table.id = returnNode.id + "_table";
                
                //inputs
                if(inputList.length > 0)
                {
                    for(var i=0; i<inputList.length; i++)
                    {
                        // get current input
                        var current = inputList[i];
                        
                        // create new row for each input
                        var row = document.createElement("tr");
                        table.appendChild(row);
                        row.className = "input";
                        
                        // create td containing the connectorDiv
                        var td = document.createElement("td");
                        td.id = returnNode.id + current.childNodes[0].nodeValue;
                        
                        // create input field
                        var setting = document.createElement("input");
                        // TODO: Set type of input
                        setting.setAttribute("type", "text");
                        setting.id = returnNode.id + "_" + current.childNodes[0].nodeValue;
                        setting.setAttribute("placeholder", current.childNodes[0].nodeValue);
                        
                        // create td for setting and append
                        td.appendChild(setting);
                        
                        row.appendChild(td);
                    }
                }
                
                //outputs
                if(outputList.length > 0)
                {   
                    for(var i=0; i<outputList.length; i++)
                    {
                        // get current output
                        var current = outputList[i];
                        
                        // create new row for each output
                        var row = document.createElement("tr");
                        row.className = "output";
                        table.appendChild(row);
                        
                        // set name of output
                        var td = document.createElement("td");
                        td.id = 
                        td.innerHTML = current.childNodes[0].nodeValue;
                        returnNode.id + "_" + current.childNodes[0].nodeValue;
                        
                        row.appendChild(td);
                    }
                }
                
                // append blockFactory
                returnNode.appendChild(table);
                if(std)
                {
                    document.getElementById("standardBlocks").appendChild(returnNode);
                }
                else
                {
                    document.getElementById("additionalBlocks").appendChild(returnNode);
                }
                
            }
            
            // AllowDrop function
            function allowDrop(ev) {
                ev.preventDefault();
            }
            
            // drag function -> save id of dragged element
            function drag(ev) {
                ev.dataTransfer.setData("text", ev.target.id);
            }
            
            // drop function of edit div
            // if blockFactory -> create block
            // if block -> change block position
            function drop(ev) {
                counter++;
                ev.preventDefault();
                var data = ev.dataTransfer.getData("text");
                var selected = document.getElementById(data);
                if(!(ev.target == selected))
               {
                    var rect = ev.target.getBoundingClientRect();
                    var x = ev.pageX - rect.left;
                    var y = ev.pageY - rect.top;

                    if(selected.className.indexOf("Factory") != -1)
                    {
                        var node = createBlock(data, x, y, ev);
                        //ev.target.appendChild(node);
                        //placeDiv(x,y,node);
                    }
               }
            }
            
            // drop function of delete div
            // delete the dropped element
            function deleteMe(id) {
                instance.remove(id);
                //var selected = document.getElementById(id);
                //selected.parentElement.removeChild(selected);
            }
            
            // create a new Block from a blockFactory
            function createBlock(data, x, y, ev)
            {
                // create new node of class "block"
                var returnNode = document.createElement("div");
                returnNode.className = "block";
                returnNode.id = data + "_" + parseInt(counter); /* We cannot use the same ID */
                ev.target.appendChild(returnNode);
                placeDiv(x,y,returnNode);

                // get the "parent" blockFactory
                var current = document.getElementById(data);
                
                // get the contents of the blockfactory
                for(var child = current.firstChild; child; child = child.nextSibling)
                {
                    // copy each content
                    var copy = child.cloneNode(true);
                    copy.id = returnNode.id + "_" + child.id;
                    returnNode.appendChild(copy);
                    var close = document.createElement("a");
                    close.setAttribute("href", "#");
                    close.onclick = function() { instance.remove(returnNode.id); };
                    close.className = "closeButton";
                    close.innerHTML = "X";
                    close.id = returnNode.id + "_close";
                    returnNode.appendChild(close);
                    
                    if(copy.nodeName == "HEADER")
                    {
                            var sourceUUID = copy.id + "connector";
                            var targetEndpoint = {
                                endpoint: "Rectangle",
                                isSource: false,
                                isTarget: true,
                                maxConnections: 1,
                                anchor: "Left",
                                endpointStyle:{ fillStyle:"red", outlineColor:"black", outlineWidth:2 }
                            }
                            instance.addEndpoint(copy, targetEndpoint);
                        
                            var targetUUID = copy.id + "connector";
                            var sourceEndpoint = {
                                endpoint: "Dot",
                                isSource: true,
                                isTarget: false,
                                maxConnections: 1,
                                anchor: "Right",
                                endpointStyle:{ fillStyle:"red", outlineColor:"black", outlineWidth:2 }
                            }
                            instance.addEndpoint(copy, sourceEndpoint);
                    }
                    
                    
                    // if the current child is the table...
                    if(copy.nodeName == "TABLE")
                    {
                        // ... loop through the rows ...
                        for(var tableChild = copy.firstChild; tableChild; tableChild = tableChild.nextSibling)
                        {   
                            // add jsplumb endpoint
                            // target endpoint
                            if(tableChild.className == "input")
                            {
                                tableChild.firstChild.id = returnNode.id + "-" + tableChild.firstChild.id;
                                var sourceUUID = copy.id + "connector";
                                var targetEndpoint = {
                                    endpoint: "Rectangle",
                                    isSource: false,
                                    isTarget: true,
                                    maxConnections: 1,
                                    anchor: "Left",
                                    endpointStyle:{ fillStyle:"white", outlineColor:"black", outlineWidth:2 }
                                }
                                instance.addEndpoint(tableChild.firstChild, targetEndpoint);
                            }
                            // source endpoint
                            else if(tableChild.className == "output")
                            {
                                tableChild.firstChild.id = returnNode.id + "-" + tableChild.firstChild.id;
                                var targetUUID = copy.id + "connector";
                                var sourceEndpoint = {
                                    endpoint: "Dot",
                                    isSource: true,
                                    isTarget: false,
                                    maxConnections: 1,
                                    anchor: "Right",
                                    endpointStyle:{ fillStyle:"white", outlineColor:"black", outlineWidth:2 }
                                }
                                instance.addEndpoint(tableChild.firstChild, sourceEndpoint);		
                            }
                        }
                    }
                }
                // make draggable
                instance.draggable(returnNode);
                return returnNode;
            }
            
            // set the position of an element
            function placeDiv(x_pos, y_pos, element) {
                element.style.position = "absolute";
                element.style.left = x_pos+'px';
                element.style.top = y_pos+'px';
            }