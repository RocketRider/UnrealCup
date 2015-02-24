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

                //_addEndpoints("for", ["TopCenter", "BottomCenter"], ["LeftMiddle", "RightMiddle"]);			
                //_addEndpoints("Window2", ["LeftMiddle", "BottomCenter"], ["TopCenter", "RightMiddle"]);
                //_addEndpoints("Window3", ["RightMiddle", "BottomCenter"], ["LeftMiddle", "TopCenter"]);
                //_addEndpoints("Window1", ["LeftMiddle", "RightMiddle"], ["TopCenter", "BottomCenter"]);

                // listen for new connections; initialise them the same way we initialise the connections at startup.
                instance.bind("connection", function(connInfo, originalEvent) { 
                    init(connInfo.connection);
                });			

                // make all the window divs draggable						
                //instance.draggable(jsPlumb.getSelector(".flowchart-demo .window"), { grid: [20, 20] });		
                // THIS DEMO ONLY USES getSelector FOR CONVENIENCE. Use your library's appropriate selector 
                // method, or document.querySelectorAll:
                //jsPlumb.draggable(document.querySelectorAll(".window"), { grid: [20, 20] });

                // connect a few up
                //instance.connect({uuids:["Window2BottomCenter", "Window3TopCenter"], editable:true});
                //instance.connect({uuids:["Window2LeftMiddle", "Window4LeftMiddle"], editable:true});
                //instance.connect({uuids:["Window4TopCenter", "Window4RightMiddle"], editable:true});
                //instance.connect({uuids:["Window3RightMiddle", "Window2RightMiddle"], editable:true});
                //instance.connect({uuids:["Window4BottomCenter", "Window1TopCenter"], editable:true});
                //instance.connect({uuids:["Window3BottomCenter", "Window1BottomCenter"], editable:true});
                //

                //
                // listen for clicks on connections, and offer to delete connections on click.
                //
                instance.bind("click", function(conn, originalEvent) {
                    if (confirm("Delete connection from " + conn.sourceId + " to " + conn.targetId + "?"))
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
                
                var blocks = xmlDoc.getElementsByTagName("block");
                for(var i=0; i < blocks.length; i++)
                {
                    var current = blocks[i];
                    var name = current.getAttribute("name");
                    var inputs = current.getElementsByTagName("input");
                    var outputs = current.getElementsByTagName("output");
                    addBlockFactory(name, inputs, outputs);
                }
            }
            
            // TODO: Create the BlockFactory for the getBlock-Div
            function addBlockFactory(name, inputList, outputList)
            {
                var returnNode = document.createElement("div");
                returnNode.className = "blockFactory";
                returnNode.id = name;
                returnNode.setAttribute("draggable", "true");
                returnNode.setAttribute("ondragstart", "drag(event)");

                var header = document.createElement("header");
                header.innerHTML = name;
                header.id = returnNode.id + "_header";
                returnNode.appendChild(header);
                
                var table = document.createElement("table");
                table.id = returnNode.id + "_table";
                
                //inputs
                if(inputList.length > 0)
                {
                    for(var i=0; i<inputList.length; i++)
                    {
                        var row = document.createElement("tr");
                        table.appendChild(row);
                        row.className = "input";
                        
                        var current = inputList[i];
                        
                        // TODO: Append connector
                        var td = document.createElement("td");
                        var connectorDiv = document.createElement("div");
                        connectorDiv.id = returnNode.id + "_" + current.childNodes[0].nodeValue + "_connector";
                        connectorDiv.className = "connectorDiv";
                        td.appendChild(connectorDiv);
                        row.appendChild(td);
                        
                        // TODO: Set type of input
                        var setting = document.createElement("input");
                        setting.setAttribute("type", "text");
                        setting.id = returnNode.id + "_" + current.childNodes[0].nodeValue;
                        setting.setAttribute("placeholder", current.childNodes[0].nodeValue);
                        
                        td = document.createElement("td");
                        td.appendChild(setting);
                        row.appendChild(td);
                    }
                }
                
                //outputs
                if(outputList.length > 0)
                {   
                    for(var i=0; i<outputList.length; i++)
                    {
                        var row = document.createElement("tr");
                        row.className = "output";
                        table.appendChild(row);
                        
                        var current = outputList[i];
                        
                        // TODO: Set type of input
                        var td = document.createElement("td");
                        td.innerHTML = current.childNodes[0].nodeValue;
                        row.appendChild(td);
                        
                        // TODO: Append connector
                        td = document.createElement("td");
                        var connectorDiv = document.createElement("div");
                        connectorDiv.id = returnNode.id + "_" + current.childNodes[0].nodeValue + "_connector";
                        connectorDiv.className = "connectorDiv";
                        td.appendChild(connectorDiv);
                        row.appendChild(td);
                    }
                }
                
                returnNode.appendChild(table);
                document.getElementById("additionalBlocks").appendChild(returnNode);
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
                    else placeDiv(x,y,selected);
               }
            }
            
            // drop function of delete div
            // delete the dropped element
            function deleteMe(ev) {
                ev.preventDefault();
                var data = ev.dataTransfer.getData("text");
                var selected = document.getElementById(data);
                selected.parentElement.removeChild(selected);
            }
            
            // TEST
            function dropCon(ev) {
                ev.preventDefault();
                var data = ev.dataTransfer.getData("text");
                var connectorA = document.getElementById(data);
                var connectorB = ev.target.getBoundingClientRect();
                
                jsPlumb.connect(connectorA, connectorB);
            }
            
            // create a new Block from a blockFactory
            function createBlock(data, x, y, ev)
            {
                var returnNode = document.createElement("div");
                returnNode.className = "block";
                returnNode.id = data + "_" + parseInt(counter); /* We cannot use the same ID */
                //returnNode.setAttribute("draggable", "true");
                //returnNode.setAttribute("ondragstart", "drag(event)");
                
                jsPlumb.draggable(returnNode);
                
                ev.target.appendChild(returnNode);
                placeDiv(x,y,returnNode);

                var current = document.getElementById(data);
                for(var child = current.firstChild; child; child = child.nextSibling)
                {
                    var copy = child.cloneNode(true);
                    copy.id = returnNode.id + "_" + child.id;
                    returnNode.appendChild(copy);
                    
                    if(copy.nodeName == "TABLE")
                    {
                        for(var tableChild = copy.firstChild; tableChild; tableChild = tableChild.nextSibling)
                        {
                            var connectors = tableChild.getElementsByClassName("connectorDiv");
                            connectors[0].id = connectors[0].id + "_" + returnNode.id;
                            if(tableChild.className == "input")
                            {
                                var sourceUUID = copy.id + "Center";
                                
                                jsPlumb.addEndpoint(connectors[0], anEndpointSource = {
                                                                        endpoint: "Rectangle",
                                                                        isSource: false,
                                                                        isTarget: true,
                                                                        maxConnections: 1,

                                                                        anchor: "Center"
                                                                    });
                            }
                            else
                            {
                                var targetUUID = copy.id + "Center";
                                jsPlumb.addEndpoint(connectors[0], anEndpointSource = {
                                                                        endpoint: "Dot",
                                                                        isSource: true,
                                                                        isTarget: false,
                                                                        maxConnections: 1,

                                                                        anchor: "Center"
                                                                    });		
                            }
                        }
                    }
                }
                
                
                return returnNode;
            }
            
            // set the position of an element
            function placeDiv(x_pos, y_pos, element) {
                element.style.position = "absolute";
                element.style.left = x_pos+'px';
                element.style.top = y_pos+'px';
            }