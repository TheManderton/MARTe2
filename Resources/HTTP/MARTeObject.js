/**
 * @file MARTeObject.js 
 * @brief Source file for class MARTeObject.js
 * @date 27/03/2019
 * @author Andre' Neto
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This source file contains the definition of all the methods for
 * the class MARTeObject (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */
/**
 * @brief Parent class of all objects that which to use the MARTe based javascript plugin viewer.
 */
class MARTeObject {
    /**
     * @brief NOOP
     */
    constructor() {
        this.lastCallId = "";
    }

    /**
     * @brief Virtual display method to be specialised by the inherited objects in order to create the display environment.
     * @details The default implementation creates a text area to show the object value in a json structure.
     * @param[in] target the target container where to display the data.
     */
    prepareDisplay(target) {
        this.textarea = document.createElement('textarea');
        this.inputRefresh = document.createElement('input');
        this.inputRefresh.value = "100";
		this.textarea.setAttribute("disabled", true);
		var textRefreshRate = document.createTextNode("Refresh rate (ms)");
		target.innerHTML = "";
		target.appendChild(textRefreshRate);
		target.appendChild(this.inputRefresh);
		target.appendChild(this.textarea);
        
        this.inputRefresh.addEventListener("change",
                    function(ev) {			
                        this.refresh(parseInt(this.inputRefresh.value));
                    }.bind(this),
                    false);
	
		var refreshInt = parseInt(this.inputRefresh.value); 
		if (!isNaN(refreshInt)) {
			this.refresh(refreshInt);
		}
    }

    /**
     * @brief Virtual display method to be specialised by the inherited objects in order to update the value of components.
     * @details The default implementation shows the object value in a json structure.
     * @param[in] jsonData the object information in json format.
     */
    displayData(jsonData) {
		this.textarea.innerHTML = JSON.stringify(jsonData, null, '\t');
    }
    
    /**
     * @brief Sets the object path.
     * @param[in] the object path.
     */
    setPath(path) {
    	this.path = path;
    }
    
    /**
     * @brief Gets the object path.
     * @return the object path.
     */
    getPath() {
    	return this.path;
    }
    
    /**
     * @brief Refreshes the current object with new data from the server.
     * @param[in] period optional refresh period in ms.
     * @param[in] callId expected to be not set or to be the same of the last setTimeout (otherwise it implies that the period was changed in-between and thus the setTimeout should be aborted).
     */
    refresh(period, callId) {
    	if (callId === undefined) {
            callId = "" + Date.now();
            this.lastCallId = callId;
        }
    	if (period === undefined) {
    		period = 0;
    	}
        if (callId === this.lastCallId) {
            //Get the URL and add all the extra parameters
            var fullURL = MARTeLoader.instance().getDataUrl(this.getPath());
            var xhttp = new XMLHttpRequest();
            var that = this;
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                    try {
                        var jsonData = JSON.parse(this.responseText);
                        that.displayData(jsonData);
                        if (period > 0) {
                            setTimeout(function() {that.refresh(period, callId);}, period);
                        }
                    }
                    catch (e) {
                        console.log(e);
                    }

                }
            };		
            xhttp.open("GET", fullURL, true);
            xhttp.send();
        }
    }
}

