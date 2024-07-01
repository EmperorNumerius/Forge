import OpenSCAD from "./openscad.js";
// OPTIONAL: add fonts to the FS
import { addFonts } from "./openscad.fonts.js";
// OPTIONAL: add MCAD library to the FS
import { addMCAD } from "./openscad.mcad.js";

// cylinder(d1=50,d2=0,h=10,$fn=1); use it as a test
export async function exportSTL(codeInput) {
    let filename = "tetrahedron.stl";
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
        return link.href;
    } catch (error) {
        console.error("Error exporting STL:", error);
        return "fail";
    }
}