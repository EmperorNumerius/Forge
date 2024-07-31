import OpenSCAD from "./openscad.js";
// OPTIONAL: add fonts to the FS
import { addFonts } from "./openscad.fonts.js";
// OPTIONAL: add MCAD library to the FS
import { addMCAD } from "./openscad.mcad.js";

// cylinder(d1=50,d2=0,h=10,$fn=1); use it as a test
export async function exportSTL(codeInput) {
    let filename = "model.stl";
    try {
        // Await the asynchronous initialization of OpenSCAD
        const instance = await OpenSCAD({noInitialRun: true});
        if (!instance || !instance.FS) {
            console.error("Failed to initialize OpenSCAD or FS is not available");
            return;
        }
        instance.FS.writeFile("/input.scad", codeInput);

        instance.callMain(["/input.scad", "--enable=manifold", "-o", filename]);
        const output = instance.FS.readFile("/" + filename);

        const link = document.createElement("a");
        link.href = URL.createObjectURL(new Blob([output], { type: "application/octet-stream" }));
        link.download = filename;
        console.log(link.href);
        console.log("filename:",filename);
        document.body.append(link);
        link.click();
        link.remove();
    } catch (error) {
        console.error("Error exporting STL:", error);
    }
}

export async function returnSTL(codeInput) {
    let filename = "tetrahedron.stl";
    let stderrMessages = []; // Initialize an array to store stderr messages

    try {
        const instance = await OpenSCAD({
            noInitialRun: true,
            printErr: function(message) { // Custom function to capture stderr messages
                stderrMessages.push(message);
            }
        });

        if (!instance || !instance.FS) {
            console.error("Failed to initialize OpenSCAD or FS is not available");
            return "Initialization failed";
        }
        instance.FS.writeFile("/input.scad", codeInput);

        instance.callMain(["/input.scad", "--enable=manifold", "-o", filename]);
        const output = instance.FS.readFile("/" + filename);

        const link = document.createElement("a");
        link.href = URL.createObjectURL(new Blob([output], { type: "application/octet-stream" }));
        link.download = filename;
        console.log(link.href);

        /* if uncommented it will always throw the same error
        if (stderrMessages.length > 0) {
            console.error("OpenSCAD stderr output:", stderrMessages.join("\n"));
            
        }
        */

        log("No errors yet!")
        return link.href;
        
    } catch (error) {
        console.error("Error rendering model:", error);
        if (stderrMessages.length > 0) {
            console.error("Additional details:", stderrMessages.join("\n"));
            log(stderrMessages.join("\n"));
        }
        let userFriendlyMessage = "An error occurred while rendering the model.";
        if (error.errno === 44) { // Example of handling a specific error code
            userFriendlyMessage += " This is a file system error.";
        }
           
        return "fail";
    }
}
function log(error, notAnError) { // for logging things to the builtin console
    // example:
    /*
    Could not initialize localization. ERROR: Parser error: syntax error in file input.scad, line 3 Can't parse file '/input.scad'!
    */
    let errorBuffer = error;

    errorBuffer = errorBuffer.replace(/line (\d+)/, function(match, lineNumber) {

        let offsetLineNumber = parseInt(lineNumber, 10) - 1;

        // we also have to correct because if the error is on line one but there are 7 lines after that then it will be on line 8
        
        const code = window.getEditorContent();
        console.log(code);
        const lines = code.split("\n");
        console.log("lines: ", lines);
        let newLineNumber = offsetLineNumber;

        console.log("line: ", offsetLineNumber);
        console.log(lines[newLineNumber]);

        while (lines[newLineNumber -1].trim() === "") {
            console.log("line: ", newLineNumber -1);
            newLineNumber--;
        }

        return "line " + newLineNumber;
    });

    errorBuffer = errorBuffer.replace(" '/input.scad'", '');
    errorBuffer = errorBuffer.replace("input.scad", '');


    document.getElementById("console").innerHTML = errorBuffer;
}